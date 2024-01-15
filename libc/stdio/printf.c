#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


static bool print(const char* data, size_t length) {
    const unsigned char* bytes = (const unsigned char*)data;
    for (size_t i = 0; i < length; i++)
        if (putchar(bytes[i]) == EOF)
            return false;
    return true;
}

static bool print_number(long long num, int base, bool is_signed, size_t width, char pad_char) {
    char buffer[21]; // Buffer to hold the string representation of the number (long long maximum length is 20 characters)
    char* ptr = &buffer[20];
    *ptr = '\0';

    // Handle negative numbers for signed types
    if (is_signed && num < 0) {
        do {
            *(--ptr) = '0' - (num % base);
            num /= base;
        } while (num != 0);
        *(--ptr) = '-';
    } else {
        do {
            *(--ptr) = '0' + (num % base);
            num /= base;
        } while (num != 0);
    }

    size_t len = strlen(ptr);

    // Print leading zeros or pad with spaces if necessary
    size_t pad_len = width > len ? width - len : 0;
    for (size_t i = 0; i < pad_len; ++i) {
        if (!print(&pad_char, 1))
            return false;
    }

    // Print the actual number
    return print(ptr, len);
}

int printf(const char* restrict format, ...) {
    va_list parameters;
    va_start(parameters, format);

    int written = 0;

    while (*format != '\0') {
        size_t maxrem = INT_MAX - written;

        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%')
                format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;
            if (maxrem < amount) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(format, amount))
                return -1;
            format += amount;
            written += amount;
            continue;
        }

        const char* format_begun_at = format++;

        // Handle format specifiers
        size_t width = 0;
        char pad_char = ' ';

        // Parse width and padding character
        if (*format == '0') {
            pad_char = '0';
            format++;
        }

        while (*format >= '0' && *format <= '9') {
            width = width * 10 + (*format - '0');
            format++;
        }

        switch (*format) {
        case 'c': {
            format++;
            char c = (char)va_arg(parameters, int);
            if (!maxrem) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(&c, sizeof(c)))
                return -1;
            written++;
            break;
        }
        case 's': {
            format++;
    		const char *str = va_arg(parameters, const char *);
		
    		// Handle NULL pointer
    		if (!str) {
    		    str = "(null)";
    		}
		
    		size_t len = strlen(str);
		
    		// Adjust the length based on the width specification
    		if (width > 0 && width > len) {
    		    len = width;
    		}
		
    		// Print leading spaces if necessary
    		size_t pad_len = width > len ? width - len : 0;
    		for (size_t i = 0; i < pad_len; ++i) {
    		    if (!print(&pad_char, 1))
    		        return -1;
    		}
		
    		// Print the actual string
    		if (!print(str, len))
    		    return -1;
			
    		written += len;
    		break;
        }
        case 'd': {
            format++;
            int num = va_arg(parameters, int);
            if (!print_number(num, 10, true, width, pad_char))
                return -1;
            written += width > 0 ? width : 1; // At least one character will be printed
            break;
        }
        case 'u': {
            format++;
            unsigned int num = va_arg(parameters, unsigned int);
            if (!print_number(num, 10, false, width, pad_char))
                return -1;
            written += width > 0 ? width : 1; // At least one character will be printed
            break;
        }
        case 'x': {
    		format++;
    		unsigned int num = va_arg(parameters, unsigned int);

    		// Calculate the length of the number in hexadecimal
    		size_t len = 1;
    		unsigned int temp = num;
    		while (temp >>= 4) {
    		    len++;
    		}

    		// Adjust the length based on the width specification
    		if (width > len) {
    		    len = width;
    		}

    		// Print leading zeros or pad with spaces if necessary
    		size_t pad_len = width > len ? width - len : 0;
    		for (size_t i = 0; i < pad_len; ++i) {
    		    if (!print(&pad_char, 1))
    		        return -1;
    		}

    		// Print the actual number in hexadecimal
    		for (int i = len - 1; i >= 0; --i) {
    		    char digit = (num >> (4 * i)) & 0xF;
    		    char hex_char = digit < 10 ? '0' + digit : 'a' + digit - 10;
    		    if (!print(&hex_char, 1))
    		        return -1;
    		}

    		written += len;
    		break;
		}
        default:
            // Unknown format specifier, treat it as a regular character
            format = format_begun_at;
            size_t len = strlen(format);
            if (maxrem < len) {
                // TODO: Set errno to EOVERFLOW.
                return -1;
            }
            if (!print(format, len))
                return -1;
            written += len;
            format += len;
            break;
        }
    }

    va_end(parameters);
    return written;
}