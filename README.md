# slog
An svn log output beautifier.

Beautifies basic svn log output, like this:
```bash
------------------------------------------------------------------------
r32 | sally | 2003-01-13 00:43:13 -0600 (Mon, 13 Jan 2003) | 1 line

Added defines.
------------------------------------------------------------------------
r31 | harry | 2003-01-10 12:25:08 -0600 (Fri, 10 Jan 2003) | 1 line

Added new file bar.c
```

Into a colorized, more streamlined format, like this:

Default format - one line, truncated with ellipsis to terminal window size

![Image](/pics/slog_def.png)

Full commit message format

![Image](/pics/slog_full.png)
