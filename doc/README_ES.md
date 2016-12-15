# Hyperboria/docs
[English](README.md)

## ¿Que es Hyperboria?

Hyperboria es la red de prueba de nodos cjdns

## ¿Que es cjdns?

Cjdns es una suite criptográfica para hacer redes mallas experimentales.

## ¿Que es notable acerca de cjdns? ¿Porque debería de usarlo?
Cjdns construye una [red malla](https://es.wikipedia.org/wiki/Red_en_malla) [IPv6](https://es.wikipedia.org/wiki/IPv6) [cifrada punto a punto](https://en.wikipedia.org/wiki/End-to-end_encryption) que utiliza el [espacio de direcciones fc00::/8](https://en.wikipedia.org/wiki/Unique_local_address).

### Cifrado

Cjdns provee un túnel cifrado que utiliza un [set de claves privada/publica](https://es.wikipedia.org/wiki/Criptograf%C3%ADa_asim%C3%A9trica) para cifrar, con [el cifrador en flujo SALSA20](https://en.wikipedia.org/wiki/Salsa20), todo lo que pasa por ello, que otorga al usuario [secrecía sucesiva perfecta](https://es.wikipedia.org/wiki/Perfect_forward_secrecy).

### Asignación de Direcciones

La ipv6 asignada a la interfaz TUN, esta compuesta de los primeros 16 bytes de el hash SHA512 de el [hash SHA512](https://es.wikipedia.org/wiki/SHA-2) de su clave publica. El par de claves es generado vía [el metodo de fuerza bruta](https://es.wikipedia.org/wiki/B%C3%BAsqueda_de_fuerza_bruta) hasta que una dirección correspondiente es encontrada que comience con el byte FC. El espacio de direcciones FC00::/8 se ha asignado como un espacio de Direcciones Locales Unicast, así que estas direcciones no deberían entrar en conflicto con direcciones IPv6 asignadas por el ICANN o cualquier otra operación convencional con la Internet.

### Topología híbrida

Cjdns fue diseñado para ser usado en una topología de [amigo de una amigo](https://en.wikipedia.org/wiki/Friend_of_a_friend). Construye una [red superpuesta](https://es.wikipedia.org/wiki/Red_superpuesta) que atraviesa la NAT ([Traducción de Direcciones de Red])(https://es.wikipedia.org/wiki/Traducci%C3%B3n_de_direcciones_de_red), exponiendo todos los puertos de cada nodo a cada otro nodo dentro de la red. Aquellos que acostumbran usar la NAT para proteger sus dispositivos pudieran encontrar esto problemático.

Hasta cierto punto se esperaba que cada persona que conectara, lo haría con solo amigos de confianza. No necesita confiar los nodos intermedios con la confidencialidad o la integridad de los paquetes, sin embargo, si usted desea restringir el acceso a solo unos servicios, es su responsabilidad el implementar reglas efectivas de acceso.

Los enlaces entre nodos pueden ser establecidos sobre enlaces [UDP](https://es.wikipedia.org/wiki/User_Datagram_Protocol) deliberados sobre la Internet, o sobre conecciones deliberadas o automáticas atraves de Ethernet, puntos de acceso inalámbricos, conexiones uno a uno, o varios protocolos de transmisión y asignación específicos de cada sistema.

### Vinificación criptográfica y enrutado

Establecer un enlace entre dos nodos incluye un proceso de autorización criptográfica, que seguido de esto, otros nodo dentro de la red pueden establecer conexiones con el nuevo nodo.

Debido a la relación entre la clave publica de cada nodo y su ipv6, y el hecho de que esta relación esta verificada antes de conectar a un nodo, un usuario puede estar seguro que si conecta con una dirección IPv6 y recibe una respuesta, entonces el nodo que responde posee el clave privada requerida para decifrar la respuesta.

Si puede del todo conectar a cjdns, entonces puedes estar seguro de que el nodo que ha conectado posee la clave privada que corresponde a la IPv6.

## ¿Como puedo involucrarme?

Si usted es nuevo a cjdns, usted deseara empezar instalándolo en algún lugar y familiarizarse por su cuenta como es que trabaja. Tome notas de todo lo que usted encuentre difícil, poco intuitivo, o pobremente documentado, y comparta su conocimiento de tal manera que la próxima persona que instale cjdns y después de leer la documentación, tenga una buena experiencia.

También, lea nuestro documento de como [contribuir - documento en ingles -] (contributing.md) para tener nota de nuestras politicas para aceptar contribuciones.
