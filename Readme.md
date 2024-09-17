# Project README

## Overview

This project is a simple command-line application that takes user input (text) and renders it into a PNG image file using custom font rendering. The program employs the following libraries:

- **`schrift`**: Used for font rendering and glyph extraction.
- **`stb_image_write.h`**: Used for writing PNG image files.
- **`stb_image.h`**: Though included, it is currently unused in this version of the project (typically used for image reading).

The program allows users to:
1. Enter text (up to 99 characters).
2. Specify a custom foreground and background color for the rendered text in the image.
3. Renders the entered text into a grayscale image and converts it into an RGB PNG file with the specified colors.

This was done with the program
![The letters of the Alphabets](https://raw.githubusercontent.com/Mandyiee/Font-Image/main/images/text.png)

## Dependencies

The following external libraries are used in this project:
- **schrift**: A small library for rendering TrueType fonts.
- **stb_image.h** and **stb_image_write.h**: Part of the STB libraries for image loading and writing.

To run the project, ensure you have these libraries set up in your environment.


## How to Run

1. **Install Required Libraries**: Ensure you have the Schrift library and STB libraries (`stb_image.h` and `stb_image_write.h`).
2. **Run the Program**: Compile the program using a C compiler (e.g., `gcc`) and run the generated executable.
3. **Input Text and Colors**: Follow the prompts to input text and specify the foreground and background colors in the `R-G-B` format (e.g., `255-0-0` for red).
4. **Check Output**: The final image will be saved as `images/text.png`.

### Compilation Example:
```bash
make
./main
```

This project offers a starting point for simple font rendering and image creation, with plenty of room for future enhancements.
