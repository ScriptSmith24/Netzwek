function Caesar_Encrypt(username, messageText) {
    messageText = messageText + username.length;
    return messageText
}

function Caesar_Decrypt(username, messageText) {
    messageText = messageText - username.length;
    return messageText
}