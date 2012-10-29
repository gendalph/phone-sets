/// \file kernel/utils/encoding_utils.cpp

#include "encoding_utils.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
const int CHAR_BUF_LEN = 1024*4;
#include <windows.h>
#endif // _WIN32

#ifdef SIMULATOR_USE_ICONV
#include <iconv.h>
#endif // SIMULATOR_USE_ICONV


// Convert string to UTF-8 if nothing is known about its encoding
int convert_unknown_string_to_utf8 (
  const char *input_string,   ///< Input string in unknown encoding
  char       *output_string,  ///< A pointer to resulting UTF-8 string
  size_t      output_size     ///< Size of output string
)
{
#ifdef SIMULATOR_USE_ICONV
  iconv_t cd;
  size_t input_counter, output_counter;
  char * input_ptr, * output_ptr;
  size_t input_len = strlen (input_string);
  size_t ret;

  // Here it is checked, that a sring is a valid UTF-8 string.
  // If not - use CP1251 for conversion.
  // If something fails - use string as is
  const unsigned int n_encodings = 2;
  const char * encoding [n_encodings] = {"UTF-8", "CP1251"};
  unsigned int i_enc;

  for (i_enc = 0; i_enc < n_encodings; i_enc++)
    {
      input_counter = input_len;
      output_counter = output_size - 1;// Leave one byte for '\0'
      input_ptr = const_cast <char *> (input_string);
      output_ptr = output_string;
      cd = iconv_open ("UTF-8", encoding[i_enc]);
      if (cd == (iconv_t) -1)
        continue; // Unsupported, skip this convertion
      ret = iconv (cd, &input_ptr, &input_counter, &output_ptr, &output_counter);
      iconv_close (cd);
      if (ret == (size_t) -1)
        continue; // Some error, skip this convertion
      *output_ptr = '\0';
      return 0;
    }

  // Last resort - return string as it is!
  strncpy (output_string, input_string, output_size);
  output_string[output_size - 1] = '\0';
  return 0;

#else // ! SIMULATOR_USE_ICONV
#ifdef _MSC_VER
  wchar_t buf[CHAR_BUF_LEN];
  int ret;

  //cbMultiByte parameter should contain the size of input_string in bytes,
  //else MultiByteToWideChar function can falsely identify cp1251 string as utf8 string.
  ret = MultiByteToWideChar (CP_UTF8, MB_ERR_INVALID_CHARS, input_string, strlen (input_string), 0, 0);
  if (ret > 0)
    // Already correct UTF-8 string
    {
      strncpy (output_string, input_string, output_size);
      output_string[output_size - 1] = '\0';
      return 0;
    }
  ret = MultiByteToWideChar (CP_ACP, 0, input_string, -1, buf, CHAR_BUF_LEN);
  if (ret > 0)
    // Correct current locale string
    {
      ret = WideCharToMultiByte (CP_UTF8, 0 /*WC_ERR_INVALID_CHARS*/, buf, -1,
                                 output_string, (int) output_size, NULL, NULL);
      if (ret > 0)
        // Successfully converted from wchar_t to UTF-8
        {
          return 0;
        }
    }

  // Last resort - return string as it is!
  strncpy (output_string, input_string, output_size);
  output_string[output_size - 1] = '\0';
  return 0;
#else // ! _MSC_VER
  strncpy (output_string, input_string, output_size);
  output_string[output_size - 1] = '\0';
  return 0;
#endif // ! _MSC_VER
#endif // ! SIMULATOR_USE_ICONV
}

// Convert UTF-8 string to current string
int convert_utf8_string_to_current_locale (
  const char *input_string,   ///< Input string in UTF-8 encoding
  char       *output_string,  ///< A pointer to resulting string
  size_t      output_size     ///< Size of output string
)
{
#ifndef _WIN32
  strncpy (output_string, input_string, output_size);
  output_string[output_size - 1] = '\0';
  return 0;
#else // _WIN32
  int ret;
  wchar_t buf[CHAR_BUF_LEN];
  ret = MultiByteToWideChar (CP_UTF8, 0, input_string, -1, buf, CHAR_BUF_LEN);
  if (ret <= 0)
    return -1;
  ret = WideCharToMultiByte (CP_ACP, 0 /*WC_ERR_INVALID_CHARS*/, buf, -1, output_string, (int) output_size, NULL, NULL);
  if (ret <= 0)
    return -1;
  return 0;
#endif // _WIN32
}

// Convert UTF-8 string to wchar_t string
int convert_utf8_string_to_wchar (
  const char *input_string,   ///< Input string in UTF-8 encoding
  wchar_t    *output_string,  ///< A pointer to resulting wchar string
  size_t      output_size     ///< Size of output string
)
{
#ifndef _WIN32
  size_t ret;
  ret = mbstowcs (output_string, input_string, output_size);
  if (ret != (size_t) -1 && ret < output_size)
    return 0;
  if (ret == (size_t) -1)
    {
      const wchar_t message[] = L"ERR_UTF8";
      memcpy (output_string, message, sizeof (message));
      fprintf (stderr, "Error converting UTF-8 to wchar_t (%s)!\n", input_string);
    }
  return -1;
#else // _WIN32
  int ret;
  ret = MultiByteToWideChar (CP_UTF8, MB_ERR_INVALID_CHARS, input_string, -1, output_string, (int) output_size);
  if (ret > 0)
    return 0;
  return -1;
#endif // _WIN32
}

/// Convert wchar_t string to UTF-8 string
/// If output_string is not enough for result, it is a error
/// \return number of bytes printed to output_string excluded terminating null if success, -1 if error.
int convert_wchar_string_to_utf8 (
  const wchar_t *input_string,   ///< Input wchar_t string
  char          *output_string,  ///< A pointer to resulting UTF-8 string
  size_t         output_size     ///< Size of output string
)
{
#ifndef _WIN32
  size_t ret;
  // Return value does not include terminating null byte
  ret = wcstombs (output_string, input_string, output_size);
  if (ret != (size_t) -1 && ret < output_size)
    // function must return int, because WideCharToMultiByte returns int
    return (int) ret;
  if (ret == (size_t) -1)
    {
      const char message[] = "ERR_UTF8";
      memcpy (output_string, message, sizeof (message));
      fprintf (stderr, "Error converting wchar_t to UTF-8!\n");
    }
  return -1;
#else // _WIN32
  int ret;
  // Return value includes terminating null byte
  ret = WideCharToMultiByte (CP_UTF8, 0 /*WC_ERR_INVALID_CHARS*/, input_string, -1, output_string, (int) output_size, NULL, NULL);
  if (ret > 0)
    return ret - 1;
  return -1;
#endif // _WIN32
}
