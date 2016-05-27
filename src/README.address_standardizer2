# Address Standardizer 2.0

This is total rewrite and redesign of the address standardizer functionality.
The source repository is located at:

    https://github.com/woodbri/address-standardizer

The build process for this:

    git clone https://github.com/woodbri/address-standardizer.git
    cd address-standardizer/src
    make
    make -f Makefile.pg help
    make PGVER=9.3 -f Makefile.pg
    sudo make PGVER=9.3 -f Makefile.pg install

There are multiple option for selecting which postgresql version you want if you have multiple versions installed.

    make [PGVER=9.2|9.3|9.4|9.5]|[PG_CONFIG=/path/to/pg_config] clean|install

if you ignore both PGVER and PG_CONFIG the make file will look in your current PATH variable. Or you can change PGVER= 9.2, 9.3, 9.4, and 9.5, in which case the path will be set for Ubuntu to /usr/lib/postgresql/<PGVER>/bin/pg_config. Or you can set PG_CONFIG=/path/to/pg_config to use one in a different location.

There is a file src/tester/test-1.sql that you can load into a new database
and it will load a test lexicon and grammar and standardize an address.

More work needs to be done to create lexicons and grammars for specific data
sets but the basic code is here. Please report issues and I'll try to help.

