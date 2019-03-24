# Alan's SDCC repo

This is the SDCC svn2git with the Fuzix useful patches added

- Allows .o for object files always
- Supports banked Z80 binaries with an external bank tool
- Puts constants in CONST not sometimes in CODE (needed for banking)
- Fix for an annoying bug that breaks sh

This SDCC is currently required for Fuzix as the current upstream has bugs
that break Fuzix (and other stuff).
