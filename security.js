function Caesar_Encrypt(username, messageText) {
    let shift = username.length;
    let encryptedMessage = "";

    for (let char of messageText) {
        encryptedMessage += String.fromCharCode(char.charCodeAt(0) + shift);
    }

    return encryptedMessage;
}

function Caesar_Decrypt(username, messageText) {
    let shift = username.length;
    let encryptedMessage = "";

    for (let char of messageText) {
        encryptedMessage += String.fromCharCode(char.charCodeAt(0) - shift);
    }

    return encryptedMessage;
}