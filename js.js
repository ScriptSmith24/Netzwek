const messagesContainer = document.getElementById('messages');
const messageInput = document.getElementById('messageInput');
const usernameInput = document.getElementById('username');
const sendButton = document.getElementById('sendButton');

sendButton.addEventListener('click', sendMessage);

messageInput.addEventListener('keypress', function(event) {
    if (event.key === 'Enter') {
        sendMessage();
    }
});

function caesarEncrypt(text, shift) {
    let encrypted = "";
    for (let i = 0; i < text.length; i++) {
        encrypted += String.fromCharCode(text.charCodeAt(i) + shift);
    }
    return encrypted;
}

function caesarDecrypt(text, shift) {
    let decrypted = "";
    for (let i = 0; i < text.length; i++) {
        decrypted += String.fromCharCode(text.charCodeAt(i) - shift);
    }
    return decrypted;
}

function sendMessage() {
    const username = usernameInput.value.trim() || 'Anonym';
    const messageText = messageInput.value.trim();

    if (messageText === '') return;

    const encryptedText = caesarEncrypt(messageText, username.length);
    const data = 'username=' + encodeURIComponent(username) + '&message=' + encodeURIComponent(encryptedText);

    fetch('/api/send', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: data
    })
    .then(response => response.json())
    .then(result => {
        messageInput.value = '';
        console.log('Nachricht gesendet:', result);
    })
    .catch(error => {
        console.error('Fehler beim senden von Nachicht:', error);
    });
}

function fetchMessages() {
    fetch('/api/messages')
        .then(response => response.json())
        .then(messages => {
            messagesContainer.innerHTML = '';

            messages.forEach(msg => {
                const decryptedText = caesarDecrypt(msg.text, msg.username.length);
                const messageElement = document.createElement('div');
                messageElement.className = 'message';

                const strong = document.createElement('strong');
                strong.textContent = msg.username + ': ';

                messageElement.appendChild(strong);
                messageElement.appendChild(document.createTextNode(decryptedText));
                messagesContainer.appendChild(messageElement);
            });

            messagesContainer.scrollTop = messagesContainer.scrollHeight;
        })
        .catch(error => {
            console.error('Fehler beim Abrufen:', error);
        });
}

fetchMessages();

setInterval(fetchMessages, 1000);

