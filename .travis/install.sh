#!/bin/bash -eux
# -e: Exit immediately if a command exits with a non-zero status.
# -u: Treat unset variables as an error when substituting.
# -x: Display expanded script commands

CURL_USER="${BINTRAY_USER}:${BINTRAY_API_KEY}"

# HCP is defined as "/tmp/hcp/bin/hcp"
HCP_VERSION="1.0.3"

cd /tmp
wget -q -O - "http://tho-otto.de/download/hcp-${HCP_VERSION}-linux.tar.bz2" | tar xjf -
mv "hcp-${HCP_VERSION}" hcp

sudo mkdir -p "/usr/m68k-atari-mint"
cd "/usr/m68k-atari-mint"
for package in mintlib fdlibm gemlib
do
	unset PACKAGE_VERSION
	unset PACKAGE_PATH
	PACKAGE_VERSION=$(curl -s -u "${CURL_USER}" -X GET "https://api.bintray.com/packages/freemint/freemint/$package" | jq -r '.latest_version')
	read PACKAGE_PATH \
		<<< $(curl -s -u "${CURL_USER}" -X GET "https://api.bintray.com/packages/freemint/freemint/$package/versions/$PACKAGE_VERSION/files" \
			| jq -r '.[].path')
	wget -q -O - "https://dl.bintray.com/freemint/freemint/$PACKAGE_PATH" | sudo tar xjf -
done
cd -
