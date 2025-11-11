=====================================================================
#!/bin/bash
docker start TPE2
docker exec -u $USER -it TPE2 make clean -C /root/Toolchain
docker exec -u $USER -it TPE2 make clean -C /root/
docker exec -u $USER -it TPE2 make -C /root/Toolchain
docker exec -u $USER -it TPE2 make -C /root/
docker stop TPE2