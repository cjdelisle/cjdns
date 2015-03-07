Autostart cjdns when you log-in
===============================

You can configure your session to autostart cjdroute when you log-in to your
computer. Just add a file in `~/.config/autostart/cjdoute.desktop` containing:

    [Desktop Entry]
    Comment=
    Terminal=false
    Name=cjdroute
    Exec=xterm -class cjdroute -e bash -c '...path/to/cjdns/cjdroute <~/.config/cjdns/cjdroute.conf; echo "Terminated ($?)"; read'
    Categories=Network
    Keywords=cjdns
    StartupWMClass=cjdroute
    Type=Application
    Icon=modem
    Version=1.0

As you can see, you should put your cjdroute configuration in
`~/.config/cjdns/cjdroute.conf`.
