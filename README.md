# Port-Scanner
A port scanner that scans a target IP for open ports and displays the results, built with C

## Features
- Scans a single port or a range of ports
- Multithreaded, utilizing 10 threads
- Supports TCP connections
- Displays open ports

## Usage
- Compile with a C compiler (make sure to include -lws2_32 when compiling from CLI)
- Pass three arguments when executing : mode, IP, and port or port range

## Example
```gcc PortScanner.c -o PortScanner.Exe -lws2_32```
then
```./PortScanner "tcp" "127.0.0.1" "1-1000"```

## License
MIT

## Planned Features
- Linux version
- SYN scanning
- UDP scanning
