=====================================================================
#!/bin/bash

sudo docker start TPE2
sudo docker exec -it TPE2 make clean -C /root/Toolchain
sudo docker exec -it TPE2 make clean -C /root/
sudo docker exec -it TPE2 make -C /root/Toolchain
sudo docker exec -it TPE2 make -C /root/