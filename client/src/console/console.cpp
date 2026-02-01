///
/// @file
///
/// @brief
/// Basic UART console implementation source
///
/// @details
/// This is the source file for the module that is responsible for handling the
/// transmission and reception of data via the console UART. It implements a
/// basic console that the user is able to interact with.


//
// Local Includes
//
#include "console.h"
#include "../misc/macros.h"
#include "../cpe/cpe.h"

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#undef BEGIN_C_DECLS
#undef END_C_DECLS
#if defined(__cplusplus)
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS }
#else
#  define BEGIN_C_DECLS /* empty */
#  define END_C_DECLS /* empty */
#endif

BEGIN_C_DECLS

//
// Local Definitions
//
#define CONSOLE_CTRL_KEY_ADD(_key,_byte,_fn) {_byte, CONSOLE_KEY_ ## _key, _fn}
#define CONSOLE_ESC_KEY_ADD(_key, _esc, _fn)             \
    {_esc, CONSOLE_KEY_ ## _key, sizeof(_esc) - 1u, _fn}

//
// Local Structures / Enumerations / Type Definitions
//

/// @brief
/// The various states of the console
///
/// @details
/// This enumeration contains all the states that the console module could
/// possibly find itself in.
enum console_action_states
{
    /// @brief
    /// Display the command prompt
    CONSOLE_STATE_DISPLAY_PROMPT,
    /// @brief
    /// Acquire command line to process
    CONSOLE_STATE_ACQUIRE_COMMAND_LINE,
    /// @brief
    /// Prepare to start processing a command line
    CONSOLE_STATE_PREPARE_PROCESS_COMMAND_LINE,
    /// @brief
    /// Process command line
    CONSOLE_STATE_PROCESS_COMMAND_LINE,
    /// @brief
    /// Display a new line then start the acquiring of a command line
    CONSOLE_STATE_NEWLINE_THEN_ACQUIRE,
    /// @brief
    /// Maximum number of console states
    CONSOLE_STATE_MAX
};

/// @brief
/// Current command line
///
/// @details
/// This structure is what the current command line the console is working on.
struct console_input
{
    /// @brief
    /// The position in the current command line to insert the next character
    size_t ipos;
    /// @brief
    /// The length of the current command line
    size_t len;
    /// @brief
    /// The maximum length the command line could be including the ending NULL
    size_t cap;
    /// @brief
    /// The current escape length
    size_t esc_pos;
    /// @brief
    /// The current escape sequence buffer
    uint8_t esc_buff[CONSOLE_ESC_BUFF_SZ];
    /// @brief
    /// The current command line buffer
    uint8_t buff[CONSOLE_MAX_LINE_SZ + 1];
};

/// @brief
/// The console private information
///
/// @details
/// This structure contains the things necessary to properly implement a basic
/// console.
struct console_info
{
    struct uart_funcs uart;
    enum console_action_states action_state;
    struct console_input * in;
    char vbuf[CONSOLE_VPRINTF_BUFF_SZ];
};

/// @brief
/// Enumeration of special keys
enum console_keys
{
    /// @brief
    /// Backspace key
    CONSOLE_KEY_BACKSPACE,
    /// @brief
    /// Key combo CTRL + C
    CONSOLE_KEY_CTRL_C,
    /// @brief
    /// Delete key
    CONSOLE_KEY_DELETE,
    /// @brief
    /// Down arrow key
    CONSOLE_KEY_DOWN,
    /// @brief
    /// End key
    CONSOLE_KEY_END,
    /// @brief
    /// Home key
    CONSOLE_KEY_HOME,
    /// @brief
    /// Insert key
    CONSOLE_KEY_INSERT,
    /// @brief
    /// Left arrow key
    CONSOLE_KEY_LEFT,
    /// @brief
    /// Keypad enter key
    CONSOLE_KEY_KP_ENTER,
    /// @brief
    /// Right arrow key
    CONSOLE_KEY_RIGHT,
    /// @brief
    /// Page down key
    CONSOLE_KEY_PGDOWN,
    /// @brief
    /// Page up key
    CONSOLE_KEY_PGUP,
    /// @brief
    /// Up arrow key
    CONSOLE_KEY_UP,
    /// @brief
    /// Maximum number of keys
    CONSOLE_KEY_MAX
};

/// @brief
/// Control keys
///
/// @details
/// This structure defines what a control key that the console supports is and
/// does.
struct ctrl_key
{
    /// @brief
    /// This is the control key code
    uint8_t byte;
    /// @brief
    /// This is the control key
    enum console_keys key;
    /// @brief
    /// This is the function to execute when the control key is detected
    void (*handle_fn)(struct uart_funcs *, enum console_keys, uint8_t);
};

/// @brief
/// Escape key
///
/// @details
/// This structure defines what an escape key that the console supports is and
/// does.
struct esc_key
{
    /// @brief
    /// This is the escape key sequence of bytes
    const char * bytes;
    /// @brief
    /// This is the escape key
    enum console_keys key;
    /// @brief
    /// This is the length of the escape key sequence bytes
    unsigned len;
    /// @brief
    /// This is the function to execute when the control key is detected
    void (*handle_fn)(struct uart_funcs *, enum console_keys, const char *);
};

//
// Local Function Prototypes
//

/// @brief
/// Get the struct console_info pointer
///
/// @details
/// Obtains the struct console_info from a given uart pointer
///
/// @param [in] uart
/// A pointer to a specific struct uart_funcs
///
/// @return
/// Returns the struct console_info that the struct uart_funcs is part of.
static inline struct console_info *
console_info_from_uart_funcs(struct uart_funcs * uart);

/// @brief
/// Initialize console_input structure
///
/// @return
/// A pointer to an initialized console_input structure
static struct console_input *
console_input_init(void);

/// @brief
/// Receive bytes
///
/// @details
/// Receive bytes from the UART and process them
///
/// @param [in] uart
/// A pointer to a specific struct uart_funcs
static void
console_recv_data(struct uart_funcs * uart);

/// @brief
/// Process UART byte
///
/// @details
/// This function processes a single byte that has been received from the UART
///
/// @param uart
/// A pointer to a specific struct uart_funcs
///
/// @param byte
/// The byte to be processed of type uint8_t
static void
console_process_byte(struct uart_funcs * uart, uint8_t byte);

/// @brief
/// Add character to command line
///
/// @brief
/// Add a character to a specific spot in the current command line
///
/// @param [in] in
/// A pointer to the struct console_input
///
/// @param [in] byte
/// This is the byte to be added to the input buffer of type uint8_t
///
/// @return
/// A integer that indicates if the function was successful or not
///
/// @retval 0
/// The byte was successfully added to the input buffer
///
/// @retval -1
/// The byte could not be added to the input buffer
static int
console_input_insert(struct console_input * in, uint8_t byte);

/// @brief
/// Delete previous character from command line
///
/// @details
/// This function performs the tasks required to implement a backspace on the
/// current command line
///
/// @param [in] uart
/// A pointer to a specific struct uart_funcs
///
/// @param [in] key
/// (NOT USED) The key that was pressed of type enum console_keys
///
/// @param [in] byte
/// (NOT USED) The byte received from the console of type uint8_t
static void
console_key_backspace(
    struct uart_funcs * uart,
    enum console_keys key,
    uint8_t byte);

/// @brief
/// Delete current character from command line
///
/// @details
/// This function performs the tasks required to implement the deletion of the
/// current character of the current command line.
///
/// @param [in] uart
/// A pointer to a specific struct uart_funcs
///
/// @param [in] key
/// (NOT USED) The key that was pressed of type enum console_keys
///
/// @param [in] byte
/// (NOT USED) The byte received from the console of type uint8_t
static void
console_key_delete(
    struct uart_funcs * uart,
    enum console_keys key);

/// @brief
/// Determine if a byte is a control byte
///
/// @details
/// This function determines if the specified byte is a control byte
///
/// @param byte
/// This is the byte to be checked of type uint8_t
///
/// @return
/// Indicates if the byte is a control byte or not
///
/// @retval 0
/// This indicates that the byte is not a control byte
///
/// @retval 1
/// This indicates that the byte us a control byte
static int
console_is_ctrl_byte(uint8_t byte);

/// @brief
/// Process a byte as a control byte
///
/// @details
/// This function processes the specified by as a control byte. It will handle
/// return or enter key presses, specified special keys presses, and start the
/// handling of specified escape sequences.
///
/// @param uart
/// A pointer to a specific struct uart_funcs
///
/// @param byte
/// This is the control byte to be processed of type uint8_t
static void
console_process_ctrl_byte(struct uart_funcs * uart, uint8_t byte);

/// @brief
/// Determine if a specified control byte is handled or not
///
/// @details
/// This function determines if the specified control byte is one that is
/// currently handled or not.
///
/// @param byte
/// This is the control byte to see if it is handled of type uint8_t
///
/// @return
/// Indicates if the specified control byte is handled or not.
///
/// @retval 0
/// The specified control byte is not handled.
///
/// @retval 1
/// The specified control byte is handled.
static int
console_is_handled_ctrl_byte(uint8_t byte);

/// @brief
/// Clear a console_input struct
///
/// @details
/// This function prepares a specified console_input structure for its next use
///
/// @param [in] in
/// A pointer to the console_input structure to be prepared
static void
console_input_clear(struct console_input * in);

/// @brief
/// Process an escape sequence byte
///
/// @details
/// This function gathers up the various escape sequence bytes and searches for
/// a supported match.
///
/// @param uart
/// A pointer to a specific struct uart_funcs
///
/// @param byte
/// The escape byte to be processed
static void
console_process_esc_byte(struct uart_funcs * uart, uint8_t byte);

/// @brief
/// Escape sequence key to be ignored
///
/// @details
/// This function doesn't do anything. It is to be paired with escape sequences
/// that currently aren't supported.
///
/// @param [in] uart
/// A pointer to a specific struct uart_funcs
///
/// @param [in] key
/// The escape key that was pressed which is a console_keys enumeration
///
/// @param [in] esc
/// A pointer to the escape sequence bytes
static void
console_esc_key_ignore(
    struct uart_funcs *uart,
    enum console_keys key,
    const char *esc);

/// @brief
/// Perform actions for various key presses
///
/// @details
/// This function handles a number of key presses that the console handles
///
/// @param [in] uart
/// A pointer to a specific struct uart_funcs
///
/// @param [in] key
/// The key that was pressed which is a console_keys enumeration
///
/// @param [in] byte
/// The byte representation of the key pressed
static void
console_key_various(
    struct uart_funcs * uart,
    enum console_keys key,
    uint8_t byte);

/// @brief
/// Perform actions for various escape sequences
///
/// @details
/// This function handles a number of escape sequence bytes that the console is
/// aware of.
///
/// @param [in] uart
/// A pointer to a specific struct uart_funcs
///
/// @param [in] key
/// The escape key that was pressed which is a console_keys enumeration
///
/// @param [in] esc
/// A pointer to the escape sequence bytes
static void
console_esc_key_various(
    struct uart_funcs * uart,
    enum console_keys key,
    const char *esc);

/// @brief
/// Display newline
///
/// @details
/// This function displays a newline on the specified UART.
///
/// @param [in] uart
/// A pointer to the uart_funcs structure
static void
console_newline(struct uart_funcs * uart);

/// @brief
/// Delete remaining line
///
/// @details
/// This function deletes the remaining part of the current command line from
/// the cursor's present postion.
///
/// @param uart
/// A pointer to the uart_funcs structure
static void
console_cutline_here(struct uart_funcs * uart);

//
// Local Global Variables
//
static struct console_info con_info;
static struct console_input con_input;
static const struct ctrl_key __ctrl_keys[] =
{
    CONSOLE_CTRL_KEY_ADD(BACKSPACE, '\b',   console_key_backspace),
    CONSOLE_CTRL_KEY_ADD(BACKSPACE, '\177', console_key_backspace),
    CONSOLE_CTRL_KEY_ADD(CTRL_C,    '\003', console_key_various),
};
static const struct esc_key __esc_keys[] =
{
    CONSOLE_ESC_KEY_ADD(DELETE,   "\033[3~", console_esc_key_various),
    CONSOLE_ESC_KEY_ADD(DOWN,     "\033[B",  console_esc_key_ignore),
    CONSOLE_ESC_KEY_ADD(END,      "\033[F",  console_esc_key_various),
    CONSOLE_ESC_KEY_ADD(HOME,     "\033[H",  console_esc_key_various),
    CONSOLE_ESC_KEY_ADD(INSERT,   "\033[2~", console_esc_key_ignore),
    CONSOLE_ESC_KEY_ADD(KP_ENTER, "\033OM",  console_esc_key_various),
    CONSOLE_ESC_KEY_ADD(LEFT,     "\033[D",  console_esc_key_various),
    CONSOLE_ESC_KEY_ADD(RIGHT,    "\033[C",  console_esc_key_various),
    CONSOLE_ESC_KEY_ADD(PGDOWN,   "\033[6~", console_esc_key_ignore),
    CONSOLE_ESC_KEY_ADD(PGUP,     "\033[5~", console_esc_key_ignore),
    CONSOLE_ESC_KEY_ADD(UP,       "\033[A",  console_esc_key_ignore)
};

//
// Code
//

void
console_action(struct uart_funcs * uart)
{
    struct console_info * info = console_info_from_uart_funcs(uart);

    if (CONSOLE_STATE_DISPLAY_PROMPT == info->action_state)
    {
        console_show_prompt(uart);
        info->action_state = CONSOLE_STATE_ACQUIRE_COMMAND_LINE;
    }
    else if (CONSOLE_STATE_ACQUIRE_COMMAND_LINE == info->action_state)
    {
        console_recv_data(uart);
    }
    else if (CONSOLE_STATE_PREPARE_PROCESS_COMMAND_LINE == info->action_state)
    {
        console_newline(uart);
        console_newline(uart);
        info->action_state = CONSOLE_STATE_PROCESS_COMMAND_LINE;
    }
    else if (CONSOLE_STATE_PROCESS_COMMAND_LINE == info->action_state)
    {
        // While processing a command line, there is nothing to do
        return;
    } //
    else if (CONSOLE_STATE_NEWLINE_THEN_ACQUIRE == info->action_state)
    {
        // Display a new line and prepare for the starting of the acquisition
        // of a new command line
        console_newline(uart);
        console_input_clear(info->in);
        info->action_state = CONSOLE_STATE_DISPLAY_PROMPT;
    }
    else
    {
        // If we get here, then something bad has gone wrong
        console_printf(
            uart,
            "\r\n\r\nWARN: Invalid portion of console_action\r\n");
        info->action_state = CONSOLE_STATE_DISPLAY_PROMPT;
    }
}

struct uart_funcs *
console_init(
    size_t (*read_fn)(void *, size_t),
    size_t (*write_fn)(const void *, size_t))
{
    struct console_info * info = &con_info;

    (void)memset(info, 0, sizeof(struct console_info));
    info->action_state = CONSOLE_STATE_DISPLAY_PROMPT;
    info->in = console_input_init();
    info->uart.read_fn = read_fn;
    info->uart.write_fn = write_fn;

    return &info->uart;
}

void
console_show_prompt(struct uart_funcs * uart)
{
    static const char prompt0[] = "\r\033[1;32m";
    static const char prompt1[] = " \033[0;00m";
    struct console_info * info = console_info_from_uart_funcs(uart);
    struct console_input * in = info->in;

    uart->write_fn(prompt0, sizeof(prompt0) - 1u);
    console_printf(uart, "%lu $", (unsigned long)0u);
    uart->write_fn(prompt1, sizeof(prompt0) - 1u);

    if (in->len) { uart->write_fn(in->buff, in->len); }

    if (in->ipos < in->len)
    {
        size_t n = (in->len - in->ipos);
        console_printf(uart, "\033[%uD", n);
    }
}

size_t
console_vprintf(struct uart_funcs * uart, const char * fmt, va_list ap)
{
    struct console_info * info = console_info_from_uart_funcs(uart);
    va_list tva;
    int n;

    // Print to vbuf
    va_copy(tva, ap);
    n = vsnprintf(info->vbuf, sizeof(info->vbuf), fmt, tva);
    va_end(tva);
    if (unlikely(n < 0)) { return -1; }

    // Truncate if needed
    if (unlikely(n >= (int)sizeof(info->vbuf)))
    {
        n = (int)sizeof(info->vbuf);
        info->vbuf[n - 1] = '\0';
    }

    // Send the data
    return uart->write_fn(info->vbuf, n);
}

size_t
console_printf(struct uart_funcs * uart, const char *fmt, ...)
{
    va_list ap;
    size_t ret;

    va_start(ap, fmt);
    ret = console_vprintf(uart, fmt, ap);
    va_end(ap);

    return ret;
}

void
console_process_command_done(struct uart_funcs * uart)
{
    struct console_info * info = console_info_from_uart_funcs(uart);

    info->action_state = CONSOLE_STATE_NEWLINE_THEN_ACQUIRE;
}

static inline struct console_info *
console_info_from_uart_funcs(struct uart_funcs * uart)
{
    return container_of(uart, struct console_info, uart);
}

static struct console_input *
console_input_init(void)
{
    struct console_input * rv = &con_input;

    (void)memset(rv, 0x0, sizeof(console_input));
    rv->cap = (size_t)CONSOLE_MAX_LINE_SZ + 1u;

    return rv;
}

static void
console_recv_data(struct uart_funcs * uart)
{
    uint8_t in_buff[CONSOLE_READ_BUFF_SZ] = { 0 };
    size_t nb;
    size_t idx;

    if (0 >= (nb = uart->read_fn(in_buff, sizeof(in_buff)))) { return; }

    for (idx = 0; idx < nb; idx++)
    {
        console_process_byte(uart, in_buff[idx]);
    }

    console_show_prompt(uart);
}

static void
console_process_byte(struct uart_funcs * uart, uint8_t byte)
{
    struct console_input * in = console_info_from_uart_funcs(uart)->in;

    if (in->esc_pos)
    {
        console_process_esc_byte(uart, byte);
    }
    else if (console_is_ctrl_byte(byte))
    {
        console_process_ctrl_byte(uart, byte);
    }
    else
    {
        console_input_insert(in, byte);
    }
}

static int
console_input_insert(struct console_input * in, uint8_t byte)
{
    size_t nshift;
    uint8_t * buff;

    // See if there is room to add a character
    if ((in->len + 1u) >= in->cap) { return -1; }

    // If not adding at end of string, move things to the right over by 1
    buff = in->buff;
    if ((nshift = in->len - in->ipos))
    {
        memmove(&buff[in->ipos + 1u], &buff[in->ipos], nshift);
    }

    // Insert the character
    buff[in->ipos] = byte;
    in->ipos++;
    in->len++;
    buff[in->len] = '\0';

    return 0;
}

static void
console_key_backspace(
    struct uart_funcs * uart,
    enum console_keys key,
    uint8_t byte)
{
    static const char ctl[] = "\b\033[K";
    struct console_input * in = console_info_from_uart_funcs(uart)->in;
    uint8_t * buff = in->buff;

    // Remove variables that aren't needed
    (void)key;
    (void)byte;

    // Are we at the beginning of the command line?
    if (!in->ipos || !in->len) { return; }

    // Are we in the middle of the command line?
    if (in->ipos < in->len)
    {
        size_t nshift = in->len - in->ipos;
        memmove(&buff[in->ipos - 1u], &buff[in->ipos], nshift);
    }

    in->len--;
    in->ipos--;
    buff[in->len] = '\0';
    uart->write_fn(ctl, sizeof(ctl) - 1u);
}

static void
console_key_delete(
    struct uart_funcs * uart,
    enum console_keys key)
{
    struct console_input * in = console_info_from_uart_funcs(uart)->in;
    uint8_t * buff = in->buff;
    size_t nshift;

    // Remove variables that aren't needed
    (void)key;

    // Are we at the end of the command line?
    if (in->ipos == in->len) { return; }

    // Are there characters to be deleted?
    if (!in->len || (in->ipos >= in->len)) { return; }

    // Are we in the middle of the command line?
    if ((nshift = in->len - (in->ipos + 1u)))
    {
        memmove(&buff[in->ipos], &buff[in->ipos + 1u], nshift);
    }

    in->len--;
    in->ipos = MIN(in->ipos, in->len);
    buff[in->len] = '\0';
    console_cutline_here(uart);
}

static int
console_is_ctrl_byte(uint8_t byte)
{
    int rv = 0;

    if (((uint8_t)0x7F == byte) ||
        (((uint8_t)0x00 <= byte) && ((uint8_t)0x1F >= byte))) { rv = 1; }

    return rv;
}

static void
console_process_ctrl_byte(struct uart_funcs * uart, uint8_t byte)
{
    struct console_info * info = console_info_from_uart_funcs(uart);
    struct console_input * in = info->in;
    size_t idx;

    // Has the enter or return key been pressed?
    if ('\r' == byte)
    {
        // TODO: Update history
        cpe_set_processing_buffer(in->buff);
        info->action_state = CONSOLE_STATE_PREPARE_PROCESS_COMMAND_LINE;
    }
    // Has an escape sequence started?
    else if ('\033' == byte)
    {
        in->esc_buff[0] = byte;
        in->esc_pos = 1u;
    }
    // Is it a control key that is currently handled?
    else if(console_is_handled_ctrl_byte(byte))
    {
        size_t idx;
        for (idx = 0u; idx < COUNT_OF(__ctrl_keys); idx++)
        {
            if (__ctrl_keys[idx].byte != byte) { continue; }

            __ctrl_keys[idx].handle_fn(uart, __ctrl_keys[idx].key, byte);
        }
    }
    else
    {
        // The control key pressed currently isn't handled. A warning could be
        // displayed but for now, do nothing.
        ;
    }
}

static int
console_is_handled_ctrl_byte(uint8_t byte)
{
    int rv = 0u;
    size_t idx;

    for (idx = 0u; idx < COUNT_OF(__ctrl_keys); idx++)
    {
        if (__ctrl_keys[idx].byte == byte)
        {
            rv = 1;
            break;
        }
    }

    return rv;
}

static void
console_input_clear(struct console_input * in)
{
    in->len = 0u;
    in->ipos = 0u;
    in->esc_pos = 0u;
    in->esc_buff[0] = '\0';
    in->buff[0] = '\0';
}

static void
console_process_esc_byte(struct uart_funcs * uart, uint8_t byte)
{
    struct console_input * in = console_info_from_uart_funcs(uart)->in;
    const char *ebuff = (char *)in->esc_buff;
    unsigned idx;

    // Is this an unmatched sequence?
    if (((in->esc_pos + 1u) >= COUNT_OF(in->esc_buff)) ||
        (((uint8_t)0x20u > byte) ||
         ((uint8_t)0x7fu < byte)))
    {
        size_t n;
        n = COUNT_OF(in->esc_buff) - 1u;
        n = MIN(in->esc_pos, n);
        in->esc_buff[n] = '\0';
        in->esc_pos = 0u;
        return;
    }

    in->esc_buff[in->esc_pos++] = byte;
    in->esc_buff[in->esc_pos] = '\0';
    for (idx = 0u; idx < COUNT_OF(__esc_keys); idx++)
    {
        if ((in->esc_pos != __esc_keys[idx].len) ||
            (strncmp(ebuff, __esc_keys[idx].bytes, in->esc_pos)))
        {
            continue;
        }

        __esc_keys[idx].handle_fn(uart, __esc_keys[idx].key, ebuff);
        in->esc_pos = 0u;
    }
}

static void
console_esc_key_ignore(
    struct uart_funcs * uart,
    enum console_keys key,
    const char *esc)
{
    (void)uart;
    (void)key;
    (void)esc;
}

static void
console_key_various(
    struct uart_funcs * uart,
    enum console_keys key,
    uint8_t byte)
{
    struct console_info * info = console_info_from_uart_funcs(uart);
    struct console_input * in = info->in;

    switch (key)
    {
        case CONSOLE_KEY_CTRL_C:
            info->action_state = CONSOLE_STATE_NEWLINE_THEN_ACQUIRE;
            break;
        default:
            console_printf(uart, "\r\nWARN: Keypress missed 0x%02x\r\n", byte);
            break;
    }
}

static void
console_esc_key_various(
    struct uart_funcs * uart,
    enum console_keys key,
    const char *esc)
{
    struct console_info * info = console_info_from_uart_funcs(uart);
    struct console_input * in = info->in;

    switch(key)
    {
        case CONSOLE_KEY_DELETE:
            console_key_delete(uart, key);
            break;
        case CONSOLE_KEY_END:
            in->ipos = in->len;
            break;
        case CONSOLE_KEY_HOME:
            in->ipos = 0u;
            break;
        case CONSOLE_KEY_KP_ENTER:
            // TODO: Update history
            cpe_set_processing_buffer(in->buff);
            info->action_state = CONSOLE_STATE_PREPARE_PROCESS_COMMAND_LINE;
            break;
        case CONSOLE_KEY_LEFT:
            if (in->ipos) { in->ipos--; }
            break;
        case CONSOLE_KEY_RIGHT:
            if (in->ipos < in->len) { in->ipos++; }
            break;
    }
}

static void
console_newline(struct uart_funcs * uart)
{
    static const char newline[] = "\r\n";
    uart->write_fn(newline, sizeof(newline) - 1u);
}

static void
console_cutline_here(struct uart_funcs * uart)
{
	static const char ctl[] = "\033[K";
    uart->write_fn(ctl, sizeof(ctl) - 1u);
}

END_C_DECLS
