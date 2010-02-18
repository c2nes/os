
# Functions to assist in make like shell scripts

target=$1

setColor () {
    echo -en '\E['${1}'m'
}

echoColor () {
    setColor $1
    echo -en "$2"
    setColor '0'
}

run () {
    doEcho=0
    case "$1" in 
        -v)
            shift
            doEcho=1
            ;;
        -p)
            shift
            noCheck=1
            ;;
        *)
            doEcho=0
            noCheck=0
            ;;
    esac


    if [ "$noCheck" == "1" ]; then
        echo $@;
        $@
        return
    fi

    echo -n $@;
    output=`$@ 2>&1`;

    if [ "$?" != "0" ]; then
        echoColor '1;30' '  ['
        echoColor '0;31' 'ERROR'
        echoColor '1;30' ']\n'
        echo "$output"
        exit 1
    else
        echoColor '1;30' '  ['
        echoColor '0;32' 'SUCCESS'
        echoColor '1;30' ']\n'
        if [ "$doEcho" == "1" -a -n "$output" ]; then
            echo "$output"
        fi
    fi
}

stale () {
    # target file : requirements
    if [ ! -f $1 ]; then
        return 0;
    fi
    if [ "`ls -t1 $@ | head -1`" != "$1" ]; then
        return 0;
    fi

    return 1;
}

rmIfExists () {
    for f in $@; do
        if [ -e $f ]; then
            run rm -rf $f
        fi
    done
}
