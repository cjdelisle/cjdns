# Installing cjdns on OpenIndiana

**currently broken by recent changes**

In order to install cjdns on an OpenIndiana system, do the following:

    sudo pkg set-publisher -p http://pkg.openindiana.org/sfe
    sudo pkg install runtime/gcc@4.6.2,5.11-0.151.1:20111222T011404Z
    sudo pkg install gnu-make
    sudo pkg install header-math
    sudo pkg install git
    sudo pkg install tuntap
    git clone git://github.com/cjdelisle/cjdns.git
    cd cjdns
    ./do

Once it has completed successfully, simply type ./cjdroute and follow the
normal instructions
