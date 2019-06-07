Router advertisement daemon (radvd)
===================================

El servicio [radvd](http://www.litech.org/radvd/) permite a un host linux actuar como router IPV6. Se encarga de enviar mensajes de *Router Advertisement* y para la correcta autooconfiguración de los nodos.

En el proyecto, el servicio radvd permite a los nodos obtener una dirección IP desde el gateway, lo cual habilita una ruta para enviar mensajes.


Producción (instalación con Docker)
-----------------------------------

Construir el contenedor. Desde el directorio radvd ejecutar.

```
$ docker build -f Dockerfile -t radvd .
```

Asumiendo que la interfaz lowpan0 está activa e instalada, para ejecutar el servicio, se debe utilizar el siguiente comando, para modo interactivo

```
$ docker run --rm -ti --name radvd --net=host --privileged radvd
```

O

```
$ docker run --rm -d --name radvd --net=host --privileged radvd
```

Para modo servicio. Los parámetros `--net=host` y `--privileged` son requeridos e indican al contenedor que use la red del host y que tiene permisos para modificar esa red, respectivamente.

Para probar conectividad con un nodo, se puede ejecutar el ejemplo de [gnrc_networkig de RIOT](https://github.com/RIOT-OS/RIOT/tree/master/examples/gnrc_networking) en una placa con radio 802.15.4 y hacer ping desde el gateway a la IP retornada por ifconfig en el nodo.

```
$ ping6 2001:dead:beef:0:7b7c:3513:33ee:cc06
PING 2001:dead:beef:0:7b7c:3513:33ee:cc06(2001:dead:beef:0:7b7c:3513:33ee:cc06) 56 data bytes
64 bytes from 2001:dead:beef:0:7b7c:3513:33ee:cc06: icmp_seq=1 ttl=255 time=13.7 ms
64 bytes from 2001:dead:beef:0:7b7c:3513:33ee:cc06: icmp_seq=2 ttl=255 time=15.5 ms
```

Producción (instalación completa)
-----------------------------

La imagen base ya contiene el kernel linux modificado listo para usar. El primer paso es seguir las [instrucciones para instalar la última version de radvd](https://github.com/RIOT-Makers/wpan-raspbian/wiki/Setup-native-6LoWPAN-router-using-Raspbian-and-RADVD), para lo cual será necesario compilarlo. A continuación se reproducen las instrucciones

### flex

Instalar dependencias

```
sudo apt-get install bison
```

Descargar y compilar flex

```
cd /opt/src
wget https://sourceforge.net/projects/flex/files/flex-2.6.0.tar.bz2
tar xjf flex-2.6.0.tar.bz2
cd flex-2.6.0
./configure
```

En caso de éxito, compilar e instalar

```
make
sudo make install
```

### radvd

Descargar archivos de proyecto

```
cd /opt/src
git clone https://github.com/linux-wpan/radvd.git -b 6lowpan
cd radvd
./autogen.sh
./configure --prefix=/usr/local --sysconfdir=/etc --mandir=/usr/share/man
```

En caso de éxito, compilar e instalar

```
make
sudo make install
```

### Configuración radvd

Crear o editar el archivo `/etc/radvd.conf` con el siguiente contenido

```
interface lowpan0
{
    AdvSendAdvert on;
    UnicastOnly on;
    AdvCurHopLimit 255;
    AdvSourceLLAddress on;

    prefix 2001:dead:beef::/64
    {
        AdvOnLink off;
        AdvAutonomous on;
        AdvRouterAddr on;
    };

    abro  fe80::1ac0:ffee:1ac0:ffee
    {
        AdvVersionLow 10;
        AdvVersionHigh 2;
        AdvValidLifeTime 2;
    };
};
```

Activar redirección de paquetes para interfaces ipv6 agregando la siguiente línea al `/etc/sysctl.conf`

```
echo net.ipv6.conf.all.forwarding=1 | sudo tee -a /etc/sysctl.conf
```

### Inicio automático durante partida del sistema

Para configurar radvd para que se inicie desde la partida del sistema debe ejecutar el archivo bootstrap_radvd.sh como root

```
sudo sh bootstrap_radvd.sh
```

Desarrollo
----------

### Setup RPI-6LowPAN BR

1) Setup de imagen genérica de RPi + 6LowPAN:

Se debe seguir la siguiente guía para configurar RPI con soporte de 6LowPAN

https://github.com/RIOT-Makers/wpan-raspbian/wiki/Create-a-generic-Raspbian-image-with-6LoWPAN-support
(Para RPI 2 y 3, es necesario seguir los pasos de compilación de kernel custom)

La conexión de OpenLabs at86rf233 se puede ver en la siguiente imagen:

![Conexión at86rf233](../docs/images/radio.jpg)

2) Configuración de la imagen como 6LowPAN Border Router:

Se debe completar la siguiente guía:
https://github.com/RIOT-Makers/wpan-raspbian/wiki/Setup-native-6LoWPAN-router-using-Raspbian-and-RADVD
La dirección IP utilizada para el prefijo es 2001:dead:beef::1/64 (prefijo 2001:dead:beef::)

4) Auto-init de radvd y configuración de tabla de rutas

Ejecutar radvd_bootstrap.sh para configurar la ejecución automática de radvd


Testing
-------

Para permitir pruebas end-to-end dentro de una misma maquina linux, se puede utilizar el servicio radvd dentro de un contenedor Docker.

Para ejecutar el servidor primero se debe construir el contenedor

```
$ docker build -f Dockerfile.test -t radvd .
```

Para ejecutar en la máquina local, primero se deben habilitar las [interfaces virtuales](https://en.wikipedia.org/wiki/Virtual_network_interface), RIOT provee herramientas para [creación de una red virtual](https://github.com/RIOT-OS/RIOT/wiki/Virtual-riot-network), con el comando tapsetup.

Lo más fácil es

```
$ RIOT/dist/tools/tapsetup/tapsetup -c 1
```

Donde el 1 indica que sólo se creará una interfaz virtual, más una interfaz bridge que será la que utilizará radvd

Para ejecutar el servicio, se debe utilizar el siguiente comando, para modo interactivo

```
$ docker run --rm -ti --name radvd --net=host --privileged radvd
```

O

```
$ docker run --rm -d --name radvd --net=host --privileged radvd
```

Para modo servicio. Los parámetros `--net=host` y `--privileged` son requeridos e indican al contenedor que use la red del host y que tiene permisos para modificar esa red, respectivamente.
