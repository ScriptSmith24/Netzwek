// Send message
document.getElementById('sendButton').addEventListener('click', sendMessage);
document.getElementById('messageInput').addEventListener('keypress', (e) => {
    if (e.key === 'Enter') {
        sendMessage();
    }
});

function sendMessage() {
    const username = document.getElementById('username').value.trim() || 'Anonym';
    const messageText = document.getElementById('messageInput').value.trim();
    if (messageText === '') return;

    const messagesContainer = document.getElementById('messages');
    const messageElement = document.createElement('div');
    messageElement.className = 'message';
    messageElement.innerHTML = `<strong>${username}:</strong> ${messageText}`;
    messagesContainer.appendChild(messageElement);
    messagesContainer.scrollTop = messagesContainer.scrollHeight;
    document.getElementById('messageInput').value = '';
}
