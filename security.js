function get_encryptionVariable(username){
    return username.length
}

function Caesar_Encrypt(username, messageText) {
    messageText = messageText + get_encryptionVariable(username);
    return messageText
}

function Caesar_Decrypt(username, messageText) {
    messageText = messageText - get_encryptionVariable(username);
    return messageText
}