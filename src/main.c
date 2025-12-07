#include "rylr_efpix.c"
#include <stdio.h>
int main() {
    Identity other={0};
    generate_identity(&node);
    generate_identity(&other);
    Send send={0};
    uint8_t packet[PACKET_SIZE]={0};
    send.identity=node;
    alias_copy(send.my_alias, "DSPTCH");
    memcpy(send.receiver_kx_public_key, other.kx_public_key, 32);
    get_timestamp(send.timestamp);
    memcpy(send.message, "HELLO", 5);
    encode(send, packet);

    char buffer[240];
    bintob85(buffer, packet, PACKET_SIZE);
    printf("%s\n", buffer);
    printf("%ld\n", strlen(buffer));
    printf("%d\n", PACKET_SIZE);

    b85tobin(packet, buffer);
    Recv recv={0};
    printf("%d\n", decode(packet, other, &recv, hash_check_and_relay, get_contact_from_alias, get_timestamp, get_age));
    printf("%s\n", recv.message);

    bintob85(buffer, node.kx_public_key, 32);
    printf("%s\n", buffer);
    bintob85(buffer, node.sign_public_key, 32);
    printf("%s\n", buffer);

    //add_contact(contacts, &contact_count, "AGENT", node.kx_public_key, node.sign_public_key, "DSPTCH");
    
    return 0;
}