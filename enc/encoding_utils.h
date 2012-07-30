#ifndef ENCODING_UTILS_H
#define ENCODING_UTILS_H

#include <stddef.h>

#define UTF8_TO_WCHAR_BUF(X,WBUF,WBUF_LEN) (convert_utf8_string_to_wchar ((X), (WBUF), (WBUF_LEN)), (WBUF))

// Convert string to UTF-8 if nothing is known about its encoding
int convert_unknown_string_to_utf8 (
  const char *input_string,   ///< Input string in unknown encoding
  char       *output_string,  ///< A pointer to resulting UTF-8 string
  size_t      output_size     ///< Size of output string
);

// Convert UTF-8 string to current string
int convert_utf8_string_to_current_locale (
  const char *input_string,   ///< Input string in UTF-8 encoding
  char       *output_string,  ///< A pointer to resulting string
  size_t      output_size     ///< Size of output string
);

// Convert UTF-8 string to wchar_t string
int convert_utf8_string_to_wchar (
  const char *input_string,   ///< Input string in UTF-8 encoding
  wchar_t    *output_string,  ///< A pointer to resulting wchar string
  size_t      output_size     ///< Size of output string
);

// Convert wchar_t string to UTF-8 string
int convert_wchar_string_to_utf8 (
  const wchar_t *input_string,   ///< Input wchar_t string
  char          *output_string,  ///< A pointer to resulting UTF-8 string
  size_t         output_size     ///< Size of output string
);

// A kind of "mbstowcs (NULL, s, 0)", but with limit of string length:
// If string_size is -1, (s) is a NULL terminating string
// Else (s) is a string of size (string_size)
inline int strwidth_utf8 (const char *s, int string_size = -1) {
  int i = 0, j = 0;
  if (string_size == -1)
    {
      while (s[i])
        {
          if ((s[i] & 0xc0) != 0x80)
            j++;
          i++;
        }
    }
  else
    {
      for (i = 0; i < string_size; i++)
        {
          if ((s[i] & 0xc0) != 0x80)
            j++;
        }
    }
  return j;
}

inline int not_first_char_in_utf8 (char c)
{
  return ((c & 0xc0) == 0x80);
}

inline char *get_next_utf8_char (const char *str)
{
  do {
    str++;
  } while (not_first_char_in_utf8 (*str));
  return (const_cast <char *> (str));
}

inline char *get_prev_utf8_char (const char *str)
{
  do {
    str--;
  } while (not_first_char_in_utf8 (*str));
  return (const_cast <char *> (str));
}

inline int compare_utf8_symbols (const char *s1, const char *s2)
{
  if (not_first_char_in_utf8 (*s1) || not_first_char_in_utf8 (*s2))
    return -2;

  do {
    if (*s1 < *s2)
      return -1;
    if (*s1 > *s2)
      return 1;
    s1++;
    s2++;
  } while (not_first_char_in_utf8 (*s1) && not_first_char_in_utf8 (*s2));
  if (!not_first_char_in_utf8 (*s1) && !not_first_char_in_utf8 (*s2))
    return 0;
  if (!not_first_char_in_utf8 (*s1) && not_first_char_in_utf8 (*s2))
    return -1;
  return 1;
}

#endif // ENCODING_UTILS_H
