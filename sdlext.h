//
// heligun: 3D flight sim shooter
// Copyright (c) 2018 Joseph Kuziel
//
// This software is MIT licensed.
//

/// @brief Prints the error message from SDL_GetError.

void sdlextPrintError();


/// @brief Reads entire text file into memory.
/// @param filename File name of file to read.
/// @return Pointer to file buffer or NULL on error. Remember to free memory
///     after done.

char* sdlextReadTextFile(
      const char* filename
);
