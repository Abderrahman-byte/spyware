$cwd=$(pwd)


cmake -DDEBUG=1 -DAUTH_PASSWORD="${AUTH_PASSWORD}" -DAUTH_USER="${AUTH_USER}" -DRMQ_HOST="${RMQ_HOST}" -DRMQ_USER="${RMQ_USER}" -DRMQ_VHOST="${RMQ_VHOST}" -DRMQ_PASSWORD="${RMQ_PASSWORD}" -S . -B ./build && cd ./build && MSBuild ALL_BUILD.vcxproj ; cd $cwd