# networkflowanony
## A Framework for Anonymizing Network Flows using (k,l)-anonymization

## Introduction
- This project is in its alpha version. Further releases will have comments and detailed work flow.
- Redis (a key-value store) is used to map real IP addresses prefixes (first 16 bit) with fake prefixes that would be used for analysis purposes.
- The script expects a file (flows.csv) with the following format `SrcAddr,DstAddr,Proto,SrcBytes,DstBytes` where SrcAddr is the source IP address,DstAddr is the destination IP address,Proto is the protocol,SrcBytes is outgoing BPP (Bytes per Packet), and DstBytes is the ingoing BPP
```
SrcAddr,DstAddr,Proto,SrcBytes,DstBytes
10.0.2.15,10.0.2.255,udp,243,0
10.0.2.15,10.0.2.255,udp,216,0
10.0.2.15,10.0.2.255,udp,276,0
10.0.2.15,10.0.2.2,arp,42,64
10.0.2.15,186.50.139.76,udp,114,0
10.0.2.15,186.18.3.72,udp,114,0
10.0.2.15,82.231.139.51,udp,114,0
```
## Deployment
0. Install Redis
1. Clone the project: `git clone https://github.com/Lekssays/networkflowanony.git`
2. Install requirements: `pip3 install -r requirements.txt`
3. Change permission: `chmod +x preprocess.sh`
4. Run the script: `./preprocess.sh flows.csv`
