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

function sendMessage() {
    const username = usernameInput.value.trim() || 'Anonym';
    const messageText = messageInput.value.trim();

    if (messageText === '') return;

    const data = 'username=' + encodeURIComponent(username) + '&message=' + encodeURIComponent(messageText);

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
                const messageElement = document.createElement('div');
                messageElement.className = 'message';
                messageElement.innerHTML = '<strong>' + msg.username + ':</strong> ' + msg.text;
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
