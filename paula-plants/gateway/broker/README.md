broker
======

El broker es un servicio que se encarga de traducir las peticiones CoAP de los nodos y retrasnmitirlas al servidor. Para hacer de broker se usa el servicio [Crosscoap](https://github.com/ibm-security-innovation/crosscoap), un servidor CoAP que traduce peticiones CoAP y las traduce a HTTP.


Instalación con Docker
----------------------

Para ejecutar el servidor primero se debe construir el contenedor

```
$ docker build -t broker .
```

Luego se puede ejecutar usando


```
$ docker run --rm -ti --env BACKEND=<server-url> --net=host broker
```

En modo interactivo, o

```
$ docker run -d --env BACKEND=<server-url> --net=host broker
```

El modificador `--net=host` permite que crosscoap pueda ver la interfaz lowpan0.


El servicio soporta las siguientes variables de ambiente

* BACKEND. Servidor HTTP donde redirigir peticiones. Por defecto *http://127.0.0.1/*
* LISTEN. Dirección donde escuchar peticiones CoAP. Por defecto *[::]:5683*
* ACCESSLOG. Archivo donde redirigir log de acceso. Por defecto */dev/stdout*
* ERRORLOG. Archivo donde redirigir log de error. Por defecto */dev/stderr*


Instalación completa
-----------

### Instalación de Go

Primero es necesario instalar go

```
apt-get install golang
```

Antes de instalar crosscoap, se debe configurar la ruta de instalación de paquetes externos y configurar las variables de ambiente para el funcionamiento correcto. Para el proyecto usaremos `/opt/go` como ruta.

```
$ sudo mkdir -p /opt/go
```

Para configurar las rutas se debe crear el archivo `/etc/profile.d/go.sh` de la siguiente manera

```
$ cat <<EOF | sudo tee /etc/profile.d/go.sh > /dev/null
export GOPATH=/opt/go
export PATH=\$PATH:\$GOPATH/bin
EOF
```

### Instalación de crosscoap

Si las rutas están configuradas, basta con ejecutar el siguiente comando

```
$ go get github.com/ibm-security-innovation/crosscoap/cmd/crosscoap
```

### Ejecutar

```
$ crosscoap -listen [::]:5683 -accesslog /dev/stdout -backend http://127.0.0.1/
```

### Instalación como servicio

Para que el servicio se ejecute al iniciar el gateway se debe configurar como servicio en el init. Para eso se deben ejecutar los siguientes comandos como usuario root.

```
$ cp crosscoap.conf /etc/crosscoap.conf && \
    cp crosscoap /etc/init.d/crosscoap && \
    chmod 755 /etc/init.d/crosscoap &&
    update-rc.d crosscoap defaults
```

El servicio usa el archivo en `/etc/crosscoap.conf` para configurar el servicio. El archivo define las siguientes variables

* BACKEND. Servidor HTTP donde redirigir peticiones. Por defecto *http://127.0.0.1/*
* LISTEN. Dirección donde escuchar peticiones CoAP. Por defecto *[::]:5683*
* ACCESSLOG. Archivo donde redirigir log de acceso. Por defecto */var/log/crosscoap-access.log*
* ERRORLOG. Archivo donde redirigir log de error. Por defecto */var/log/crosscoap-error.log*

Testing
-------

Para permitir pruebas end-to-end dentro de una misma maquina linux, se puede utilizar el servicio crosscoap dentro de un contenedor Docker.

Para ejecutar el servidor primero se debe construir el contenedor

```
$ docker build -f Dockerfile.test -t broker .
```

Luego se puede ejecutar usando


```
$ docker run --rm -ti --env BACKEND=<server-url> broker
```

El servicio soporta las siguientes variables de ambiente

* BACKEND. Servidor HTTP donde redirigir peticiones. Por defecto *http://127.0.0.1/*
* LISTEN. Dirección donde escuchar peticiones CoAP. Por defecto *[::]:5683*
* ACCESSLOG. Archivo donde redirigir log de acceso. Por defecto */dev/stdout*
* ERRORLOG. Archivo donde redirigir log de error. Por defecto */dev/stderr*
