# MQTT Protocol

The client subscribes to `/bitraf/door/$door_id/open` where `door_id`
identifies the particular door. To open the door send any non-empty
message to the topic.

# Bitraf Deployment

The broker and subscriber uses certificates to authenticate each other
and the broker allows only a single certificate to write to the topic.
