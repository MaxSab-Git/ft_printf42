*This project has been created as part of the 42 curriculum by mtoudic*

<h1>Description</h1>

This project aim to reimplement 'printf' from the standard C library.<br>

It only supports %cspdiuxX.<br>

<h1>Instructions</h1>

To compile, launch 'make' in your terminal and then 'make clean' to remove all *.o files, This should compile fine on most POSIX compliant systems.<br>
To use it, include 'ft_printf.h' in your *.c file and add '-lftprintf -lft' in your compiler options.<br>
Mind that 'ft_printf' depends on 'libft' and can't be used without it.<br>

<h1>Resources</h1>

References for 'ft_printf' are taken from the [printf(3) linux man pages documantaton](https://man7.org/linux/man-pages/man3/printf.3.html)<br>

<h1>Design choices</h1>

The original 'printf' probably use a static malloc'd memory to buffer the output, persisting between calls, before sending it to the console, allowing a reduction of syscalls. Unfortunaly, to safely free this memory, you must (probably) use 'atexit' or equivalent, forbidden in the context of this projet.<br>
Thus, to remedy this and make the code easier to write, I use an algorithm who's goal is to send a chunk of 'char' before a format specifier, convert the format specifier, send it, and repeat, without allocating any memory whatsoever.<br>
It's not the pretiest but it avoid having to manage buffer overflow and malloc errors, making the code less error prone.<br>
