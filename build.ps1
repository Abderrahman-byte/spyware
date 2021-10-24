$cwd=$(pwd)

$rmq_credentials="-DRMQ_HOST='snake.rmq2.cloudamqp.com' -DRMQ_USER='khqynuqy' -DRMQ_VHOST='khqynuqy' -DRMQ_PASSWORD='nwGFT0HS8QLDtGK0drCuDqMjQnyJW4L0'"

cmake $rmq_credentials -DAUTH_PASSWORD='1913@war' -DAUTH_USER='abderrahman' -S . -B ./build && cd ./build && MSBuild ALL_BUILD.vcxproj ; cd $cwd