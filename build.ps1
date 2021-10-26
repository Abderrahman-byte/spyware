$cwd=$(pwd)

$RMQ_PASSWORD="nwGFT0HS8QLDtGK0drCuDqMjQnyJW4L0"
$RMQ_VHOST="khqynuqy"
$RMQ_USER="khqynuqy"
$RMQ_HOST="snake.rmq2.cloudamqp.com"
$AUTH_USER="abderrahman"
$AUTH_PASSWORD="1913@war"

cmake -DDEBUG=1 -DAUTH_PASSWORD="${AUTH_PASSWORD}" -DAUTH_USER="${AUTH_USER}" -DRMQ_HOST="${RMQ_HOST}" -DRMQ_USER="${RMQ_USER}" -DRMQ_VHOST="${RMQ_VHOST}" -DRMQ_PASSWORD="${RMQ_PASSWORD}" -S . -B ./build && cd ./build && MSBuild ALL_BUILD.vcxproj ; cd $cwd