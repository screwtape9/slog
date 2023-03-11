# slog
## An svn log output beautifier.

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

## How to use
You pipe the output of `svn log --xml` into slog. Note the `--xml` option; it depends on that.

### Basic usage
`svn log --xml | slog`

I create and export a bash function that takes the number of commits I want to see, like so:
```bash
SLOG() {
  if [ "$#" -ne 1 ]; then
    echo "usage: SLOG <num commits>"
  else
    if which slog > /dev/null 2>&1; then
      svn log -l${1} --xml | slog
    else
      echo "Can't find slog"
    fi
  fi
}

export -f SLOG
```
Then I can just do this:
```bash
SLOG 15
```
