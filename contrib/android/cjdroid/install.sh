#!/system/bin/sh

# Config and runtime directory (if changed, also change in cjdaemon and 99cjdroute)
CJDPATH="/sdcard/cjdns"

# Partition/disk the $BINDIR and $INITDIR are on
SYSDISK="/system"

# The folder to place executables (should be in $PATH)
BINDIR="$SYSDISK"/bin

# The init.d folder the version of Android on your device expcets to see init.d scripts
INITDIR="$SYSDISK"/etc/init.d

# Change to the script directory
if ! cd "$(dirname -- "$0")"; then
    printf "Error: cannot cd to directory of command %s\n" "$0"
    exit 1
fi

# Set the name of this script
APPNAME="${0##*/}"

_sysrw() {
    # Remount $SYSDISK read/write if necessary
    mount | grep " $SYSDISK " | grep ro >/dev/null \
        && REMOUNT=1 \
        && mount -o rw,remount "$SYSBLOCK" "$SYSDISK"

    if [ "$REMOUNT" = 1 ]; then
        mount | grep " $SYSDISK " | grep ro >/dev/null \
            && printf "Error: couldn't remount %s read/write\n\n" "$SYSDISK" \
            && exit 1
    fi
}

_sysro() {
    # Remount $SYSDISK read-only if it was previously remounted
    if [ "$REMOUNT" = 1 ]; then
        mount | grep " $SYSDISK " | grep rw >/dev/null \
            && mount -o ro,remount "$SYSBLOCK" "$SYSDISK"

        mount | grep " $SYSDISK " | grep rw >/dev/null \
            && echo "Error: couldn't remount ${SYSDISK} read-only"
    fi
}

# The help output functionality
if [ -n "$1" ]; then
    if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
        printf "\nUsage:\n\t%s [option]\n\n" "$APPNAME"
        printf "\t* This script requires root permissions to run\n"
        printf "\t* Run this script with no arguments to install cjdns\n"
        printf "\t* Install cjdns again after flashing new/upated ROMs\n"
        printf "\nOptions:\n"
        printf "\t-u|--uninstall: uninstall from %s\n" "$SYSDISK"
        printf "\t-h|--help: display this help output\n\n"
        exit 0
    fi
fi

# Fail if user isn't root (it's needed from here on out)
if [ "$(whoami)" != "root" ]; then
    echo
    echo "Error: this script must be run as root"
    echo
    exit 1
fi

# Check that /dev/net/tun exists and create it if it doesn't
if [ ! -c /dev/tun ]; then
    mknod /dev/tun c 10 200
fi

# Set the device block for the system partition
SYSBLOCK=$(mount | grep " $SYSDISK " | grep -oe "^[^\ ]*" | head -n 1)
if [ ! -e "$SYSBLOCK" ]; then
    echo
    echo "Error: Couldn't detect the device ${SYSDISK} is mounted on"
    echo
    exit 1
fi

# Stop cjdroute if it's running
if [ "$(pgrep cjdroute | wc -l)" -gt 0 ]; then
    echo
    echo "Killing cjdroute..."
    echo
    killall cjdroute
fi

# The uninstall functionality and a catch for invalid arguments
if [ -n "$1" ]; then
    if [ "$1" = "-u" ] || [ "$1" = "--uninstall" ]; then
        # Remount the system partition read/write
        _sysrw

        echo "Removing cjdroid files from the ${SYSDISK} partition..."

        # Remove cjdaemon
        if [ -f "$BINDIR"/cjdaemon ]; then
            rm -rf -- "$BINDIR"/cjdaemon
        else
            echo " Warning: ${BINDIR}/cjdaemon is not present to be removed"
        fi

        # Remove cjdctl
        if [ -f "$BINDIR"/cjdctl ]; then
            rm -rf -- "$BINDIR"/cjdctl
        else
            echo " Warning: ${BINDIR}/cjdctl is not present to be removed"
        fi

        # Remove cjdroute
        if [ -f "$BINDIR"/cjdroute ]; then
            rm -rf -- "$BINDIR"/cjdroute
        else
            echo " Warning: ${BINDIR}/cjdroute is not present to be removed"
        fi

        # Remove 99cjdroute
        if [ -f "$INITDIR"/99cjdroute ]; then
            rm -rf -- "$INITDIR"/99cjdroute
        else
            echo " Warning: ${INITDIR}/99cjdroute is not present to be removed"
        fi

        # Remount the system partition read-only
        _sysro

        # Exit successfully if all the cjdroid files are gone, otherwise complain
        if [ ! -f "$BINDIR"/cjdaemon ] && [ ! -f "$BINDIR"/cjdctl ] &&
           [ ! -f "$BINDIR"/cjdroute ] && [ ! -f "$INITDIR"/99cjdroute ]; then
            echo
            echo "Uninstallation successfully completed!"
            echo
            exit 0
        else
            echo "Error: not all files were removed"
            echo
            exit 1
        fi
    else
        echo
        echo "Error: ${1} is not a valid argument"
        echo
        exit 1
    fi
fi

# Remount the system partition read/write
_sysrw

# Copy cjdns-related-files to $SYSDISK
echo
echo "Copying files to the ${SYSDISK} partition..."
if [ ! -f "files/cjdaemon" ] || [ ! -f "files/cjdctl" ] ||
   [ ! -f "files/cjdroute" ] || [ ! -f "files/99cjdroute" ]; then
    echo "Error: one or more of the required files in 'files/' are missing"
    echo
    exit 1
fi
install -D -m755 files/cjdaemon "$BINDIR"/cjdaemon
install -D -m755 files/cjdctl "$BINDIR"/cjdctl
install -D -m755 files/cjdroute "$BINDIR"/cjdroute
install -D -m755 files/99cjdroute "$INITDIR"/99cjdroute

# Remount the system partition read-only
_sysro

# Exit successfully if all the files are where they should be, otherwise complain
if [ -f "$BINDIR"/cjdaemon ] && [ -f "$BINDIR"/cjdctl ] &&
   [ -f "$BINDIR"/cjdroute ] && [ -f "$INITDIR"/99cjdroute ]; then
    # Create the config directory if it doesn't already exist
    if [ ! -d "$CJDPATH" ]; then
        echo
        echo "Creating cjdns config folder @ ${CJDPATH}..."
        echo
        install -d "$CJDPATH"
    fi

    # Generate a config file if the user doesn't already have one
    if [ ! -f "$CJDPATH"/cjdroute.conf ]; then
        echo
        echo "Creating cjdns config file @ ${CJDPATH}/cjdroute.conf..."
        echo
        /system/bin/cjdroute --genconf > "$CJDPATH"/cjdroute.conf
    fi

    echo
    echo "Installation successfully completed!"
    echo " Ensure ${CJDPATH}/cjdroute.conf is configured then"
    echo " run 'cjdctl enable' to start the cjdroute service"
    echo
    exit 0
else
    echo "Error: not all files were successfully copied"
    echo
    echo " Hint: Try rebooting then re-running this script"
    echo
    exit 1
fi
