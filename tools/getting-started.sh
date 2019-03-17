#!/bin/sh

if [[ "$1" = "" ]] ; then
    echo "Usage: getting-started.sh port"
    echo "    port - postgresql server port"
    echo ""
    echo "Assumes -U postgres -h localhost"
    echo "Assumes current directoy is address-standardizer"
    echo "Creates a database as_test, installs address_standardizer2 extension"
    echo "Loads the sample lexicons and grammars, and some test data"
    echo "Standardizes a few addresses to make sure things are working."
    exit
fi

PORT=$1

createdb -U postgres -h localhost -p $PORT as_test
psql -U postgres -h localhost -p $PORT as_test -c "create extension address_standardizer2"
tools/load-lex-gmr-files -c sample data/sample sample-lex-gmr.sql
psql -U postgres -h localhost -p $PORT as_test -f sample-lex-gmr.sql
psql -U postgres -h localhost -p $PORT as_test -f data/test-data-us.sql
psql -U postgres -h localhost -p $PORT as_test -f data/address-standardizer-test.sql

