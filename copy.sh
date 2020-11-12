container_id="d122a8ce3dfe"

docker cp $container_id:/build/ultra96v2_oob_2019_2/images/linux/BOOT.BIN .
docker cp $container_id:/build/ultra96v2_oob_2019_2/images/linux/image.ub .
docker cp $container_id:/build/ultra96v2_oob_2019_2/images/linux/system.dtb .
docker cp $container_id:/build/ultra96v2_oob_2019_2/images/linux/rootfs.tar.gz .

