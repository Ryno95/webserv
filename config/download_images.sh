# This file will download some images to store in /root/images of the webserver, so the repo will not become extremely large

mkdir -p ../root/images/

curl https://images.wallpapersden.com/image/download/planet-rising-over-galaxy_bGplZWeUmZqaraWkpJRsa21lrWloZ2U.jpg > root/images/galaxy.jpg
curl https://images.wallpapersden.com/image/download/amazing-sunset-over-lake-8k_a2pobG2UmZqaraWkpJRsa21lrWloZ2U.jpg > root/images/lake.jpg
curl https://images.wallpapersden.com/image/download/10k-glowing-abstract-shapes-art_bWVobmeUmZqaraWkpJRsa21lrWloZ2U.jpg > root/images/abstract.jpg
