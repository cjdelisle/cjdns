Configurando cjdns
=================

En este documento vamos a revisar cómo configurar cjdns y qué significa exactamente cada opción. Cuide que este es un documento vivo y que este software está aún en etapa alfa, así que las cosas están sujetas a cambio.

Empecemos con la parte superior del archivo. Primero debe notar que es formato JSON, excepto que este JSON contiene comentarios. Técnicamente eso no es valido pero también es común. Cjdns ignora los comentarios antes de interpretarlo, así que no se preocupe por ellos.

Sus Llaves y Dirección
---------------------

La parte superior del archivo especifica donde el ejecutable cjdns se encuentra, su llave de cifrado, y su dirección IPv6 de cjdns.

````javascript
{
    // The path to the cjdns core executable.
    "corePath": "/opt/cjdns/cjdns",

    // Private key:
    // Your confidentiality and data integrity depend on this key, keep it secret!
    "privateKey": "823e4EXAMPLEEXAMPLEEXAMPLEEXAMPLEEXAMPLEEXAMPLEEXAMPLEEXAMPLEc70",

    // This key corresponds to the public key and ipv6 address:
    "publicKey": "u2jf87mgqlxfzdnywp60z3tx6tkulvgh2nyc2jk1zc69zzt2s8u0.k",
    "ipv6": "fcff:a215:1e7b:a4e9:c00d:0813:93b3:7c87",
````

- `corePath`: Esto específica donde el núcleo ejecutable de cjdns se encuentra. Si descargo el código fuente a /opt/cjdns, entonces la opción defacto está bien. Si lo descargo en algún otro lado, como su directorio home por ejemplo, entonces esto tendrá que ser cambiado de acuerdo a ello.
- `privateKey`: Su llave privada es la parte del sistema que se encarga que todos los datos que llegan y salen de su computadora estén cifrados. Debe de proteger su llave privada. No la comparta.
- `publicKey`: La llave pública es lo que su computadora le da a otras computadoras para cifrar datos con ella. Estos datos cifrados solo se pueden acceder con la llave privada, de esa manera nadie puede ver el contenido de su información mientras transita por la red.
- `ipv6`: Esta es su dirección IP en la red cjdns. Es única de usted y es creada de forma segura por medio de un hash de la llave pública.

Cjdns provee los campos publicKey e ipv6 por conveniencia, pero realmente no los requiere. Si por alguna razón usted quiere quitarlos del archivo de configuración, usted puede hacerlo. Al arrancar, la llave privada será usada para generar la correspondiente llave pública, y la llave pública será usada para encontrar la correspondiente dirección ipv6. Sus relaciones son deterministas, y el solo pasar el atributo más fundamental evita errores de configuración.

Conexiones Entrantes
--------------------

La sección de `authorizedPasswords` es el área para permitir a la gente conectar hacia usted. Cualquier sistema que presente una contraseña valida, le será permitido el acceso.
**NOTA:** Estas contraseñas deben de ser largas y elegidas al azar. No hay motivo para hacerlas cortas, con palabras fáciles de recordar, porque sólo serán utilizadas por cjdns.

````javascript
    // Anyone connecting and offering these passwords on connection will be allowed.
    //
    // WARNING: Currently there is no key derivation done on the password field,
    //          DO NOT USE A PASSWORD HERE use something which is truly random and
    //          cannot be guessed.
    // Including a username in the beginning of the password string is encouraged
    // to aid in remembering which users are who.
    //
    "authorizedPasswords":
    [
        // A unique string which is known to the client and server.
        {"password": "zxl6zgxpl4stnuybdt0xlg4tn2cdl5h", "user": "default-login"}

        // More passwords should look like this.
        // {"password": "10ru8br0mhk25ccpvubv0sqnl7kuc6s", "user": "my-second-peer"},
        // {"password": "y68jm490dztxn3d2gvuv09bz55wqmjj", "user": "my-third-peer"},
        // {"password": "bnpphnq205v8nf2ksrs1fknfr572xzc", "user": "my-fourth-peer"},

        // These are your connection credentials
        // for people connecting to you with your default password.
        // adding more passwords for different users is advisable
        // so that leaks can be isolated.
        //
        // "your.external.ip.goes.here:33808":{"login": "default-login", "password":"zxl6zgxpl4stnuybdt0xlg4tn2cdl5h","publicKey":"u2jf87mgqlxfzdnywp60z3tx6tkulvgh2nyc2jk1zc69zzt2s8u0.k"}
    ],
````
- `password`: Esta es la contraseña que otro sistema puede darle a su nodo para permitirle conectar. Debe de colocarla en la sección de `password` en la siguiente parte.
  + los bloques de `password` se les puede asignar un segundo atributo `user`. En la ejecución, cuando se usa el script de `peerStats`, los nodos que han conectado con una contraseña específica le será asociado su correspondiente cadena `user`.
- `your.external.ip.goes.here:33808` Esta sección es lo que usted le daría a un amigo para que pueda conectar con usted. Le sugerimos que además le adicione la sección de `"name":"so-and-so"` (¡no olvide las comas entre secciones!) así le será más sencillo saber quien conecta con usted. Si después resulta que usted ya no desea que este usuario conecte con usted, entonces es un asunto tan simple como encontrarle y borrarle o comentarle en la linea correcta.

Interfaz de Administración
---------------

La seccion de `admin ` define las configuraciones para la interfaz administrativa de cjdns. Muchos de los scripts en `/contrib/` usan esta interfaz para interactuar con cjdns. Probablemete usted no necesita algo de aqui a solo que este ayudando a probar algo.

````javascript
    // Settings for administering and extracting information from your router.
    // This interface provides functions which can be called through a TCP socket.
    "admin":
    {
        // Port to bind the admin RPC server to.
        "bind": "127.0.0.1:11234",

        // Password for admin RPC server.
        "password": "j6mukf2khplcgpbzz0kulb8hu0xq2v9"
    },
````

- `bind`: Esto le dice a cjdns que IP y puerto la interfaz administrativa debe tomar. Como usted no deseara que otra persona desconocida esté conectando a su interfaz administrativa, probablemente será buena idea dejar esto tal y como esta.
- `password`: Este es la contraseña que será necesaria para algunas de las cosas que se pueden hacer por la interfaz administrativa. Si usted desea exponer la interfaz administrativa a la red, entonces debería de asignar una contraseña como la aquí mostrada. Si esta escuchando solo una dirección local, entonces puede usar`"NONE"` como contraseña. Este es un nuevo comportamiento en la rama `crashey`, de tal manera de ofrecer una configuración defacto mas sencilla para trabajar.

Interface(s) de conexión
-----------------------

Esto especifica las configuraciones para las interfaces de conexión de su nodo. En este momento la mayoría de la gente usa `UDPInterface` para conectar a otros nodos cjdns por medio de la internet u otras redes tradicionales. Usted también puede usar `ETHInterface` para conectar físicamente a otra maquina. Note usted que esto ultimo no es una conexión TCP/IP como a la que usted está acostumbrado.

````javascript
    // Interfaces to connect to the switch core.
    "interfaces":
    {
        // The interface which connects over UDP/IP based VPN tunnel.
        "UDPInterface":
        [
            {
                // Bind to this port.
                "bind": "0.0.0.0:33808",

                // Nodes to connect to.
                "connectTo":
                {
                    // Add connection credentials here to join the network
                    // Ask somebody who is already connected.
                }
            }
        ]

        /*
        "ETHInterface":
        [
            {
                // Bind to this device (interface name, not MAC etc.)
                "bind": "eth0",

                // Auto-connect to other cjdns nodes on the same network.
                // Options:
                //
                // 0 -- Disabled.
                //
                // 1 -- Accept beacons, this will cause cjdns to accept incoming
                //      beacon messages and try connecting to the sender.
                //
                // 2 -- Accept and send beacons, this will cause cjdns to broadcast
                //      messages on the local network which contain a randomly
                //      generated per-session password, other nodes which have this
                //      set to 1 or 2 will hear the beacon messages and connect
                //      automatically.
                //
                "beacon": 2,

                // Node(s) to connect to manually.
                "connectTo":
                {
                    // Credentials for connecting look similar to UDP credentials
                    // except they begin with the mac address, for example:
                    // "01:02:03:04:05:06":{"password":"a","publicKey":"b"}
                }
            }
        ]
        */
    },
````

- `UDPInterface`:
    - `bind`: Esto le dice a cjdns que IP y puerto usar para escuchar conexiones.
    - `connectTo`: Esto es donde usted coloca las configuraciones de conexión para los nodos a los que usted desee conectar. El formato para hacer esto suele verse como esto,
        "12.34.56.78:12345":
        {
            "password": "thisIsAnExampleOfAPassword",
            "publicKey": "z4s2EXAMPLEPUBLICKEYEXAMPLEPUBLICKEYEXAMPLEKEY4yjp0.k"
        },
    Es importante observar que otras personas pudieran colocar otros campos como`node`, pero solo `password` y `publicKey` son actualmente interpretados por cjdns.
    - archivos de configuración generados más recientemente también tienen un segundo bloque dentro de la sección `UDPInterface`, que es para para usar conexiones UDP sobre IPv6. Estos bloques se verán como esto,
        "[2001:db8::2:1]:12345":
        {
            "password": "thisIsAnExampleOfAPassword",
            "publicKey": "z4s2EXAMPLEPUBLICKEYEXAMPLEPUBLICKEYEXAMPLEKEY4yjp0.k"
        },
- `ETHInterface`:
    - `bind`: Esto le dice a cjdns que dispositivo la ETHInterface debe tomar. Esto puede ser distinto dependiendo su sistema.
    - `connectTo`: El connectTo para la ETHInterface funciona practicamente como funciona con UDPInterface, solo que en lugar de una dirección IP y un puerto, es una dirección MAC.
    - `beacon`: Esto controla el auto descubrimiento de nodos. Cambie a 0 para desactivar el auto descubrimiento de nodos, 1 para usar las emisiones de contraseñas de auto conexión de nodos contenidas en los mensajes "beacon" de otros nodos, y 2 para emitir y aceptar "beacons".
    - En versiones preliminares de cjdns, era necesario descomentar ETHInterface si usted quería usarla, sin embargo, ahora está descomentada por defacto en la rama `crashey` que eventualmente se integrará al master.

Router
------

Esto es donde usted configura las opciones de ruteo de su nodo cjdns.

````javascript
    // Configuration for the router.
    "router":
    {
        // The interface which is used for connecting to the cjdns network.
        "interface":
        {
            // The type of interface (only TUNInterface is supported for now)
            "type": "TUNInterface"

            // The name of a persistent TUN device to use.
            // This for starting cjdroute as its own user.
            // *MOST USERS DON'T NEED THIS*
            //"tunDevice": "tun0"
        },
````

- `type`: Esto especifica el tipo de interfaz que cjdns debe de usar para conectar la red. Solo TUNInterface esta soportada por el momento.
- `tunDevice`: Esto el dispositivo TUN que cjdns debe de usar para conectar la red. La mayoría de los usuarios no necesitan esto.

Túneles de IP
------------

Los túneles de IP le permitirá conectar desde la red cjdns a otra red externa. Esto aun es un trabajo en progreso; aunque esto funciona, requiere un tanto de configuracion manual de ambos lados para hacerlo util.
````javascript
        // System for tunneling IPv4 and ICANN IPv6 through cjdn which will eventually be merged to master..
        // This is using the cjdns switch layer as a VPN carrier.
        "ipTunnel":
        {
            // Nodes allowed to connect to us.
            // When a node with the given public key connects, give them the
            // ip4 and/or ip6 addresses and prefix lengths listed.
            "allowedConnections":
            [
                // Give the client an address on 192.168.1.0/24, and an address
                // it thinks has all of IPv6 behind it.
                // {
                //     "publicKey": "f64hfl7c4uxt6krmhPutTheRealAddressOfANodeHere7kfm5m0.k",
                //     "ip4Address": "192.168.1.24",
                //     "ip4Prefix": 24,
                //     "ip6Address": "2001:123:ab::10",
                //     "ip6Prefix": 0
                // },

                // It's ok to only specify one address.
                // {
                //     "publicKey": "ydq8csdk8p8ThisIsJustAnExampleAddresstxuyqdf27hvn2z0.k",
                //     "ip4Address": "192.168.1.25",
                //     "ip4Prefix": 24
                // }
            ],

            "outgoingConnections":
            [
                // Connect to one or more machines and ask them for IP addresses.
                // "6743gf5tw80ExampleExampleExampleExamplevlyb23zfnuzv0.k",
                // "pw9tfmr8pcrExampleExampleExampleExample8rhg1pgwpwf80.k",
                // "g91lxyxhq0kExampleExampleExampleExample6t0mknuhw75l0.k"
            ]
        }
    },
````

- `allowedConnections`:
- `outgoingConnections`:

Misceláneas
-------------

Esta parte contiene sobre la sección de seguridad para configurar las opciones del programa y algunas otras cosas misceláneas que no cuadran en alguna otra categoría general.

````javascript
    // Dropping permissions.
    "security":
    [
        // Set number of open files to zero, in Linux, this will succeed even if
        // files are already open and will not allow any files to be opened for the
        // duration of the program's operation.
        // Most security exploits require the use of files.
        "nofiles",

        // Change the user id to this user after starting up and getting resources.
        {"setuser": "nobody"}
     ],
}
````

