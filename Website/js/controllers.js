/**
 * js/controllers.js
 * * Contains the "setup" logic for each page.
 * This is the "glue" that connects the UI to the API and state.
 */

import { api } from './api.js';
import { auth } from './auth.js';
import { ui } from './ui.js';
import { state } from './state.js';
import { router } from './router.js';

let messagePollingInterval = null;

function cleanupChatPolling() {
    if (messagePollingInterval) {
        clearInterval(messagePollingInterval);
        messagePollingInterval = null;
        console.log('Message polling stopped.');
    }
}

async function setupAddUserModal() {
    ui.renderUserListInModal([]);
    document.getElementById('modal-user-list').innerHTML = `<p class="text-center text-gray-500">Загрузка...</p>`;

    try {
        if (state.allUsers.length === 0) {
            const data = await api.getUsers();
            state.allUsers = data.users || [];
        }
        ui.renderUserListInModal(state.allUsers);
    } catch(error) {
        document.getElementById('modal-user-list').innerHTML = `<p class="text-center text-red-500">Не удалось загрузить список пользователей.</p>`;
        console.error(error);
        return;
    }
    
    const searchInput = document.getElementById('user-search-input');
    searchInput.addEventListener('input', (e) => {
        ui.renderUserListInModal(state.allUsers, e.target.value);
    });

    const userListEl = document.getElementById('modal-user-list');
    userListEl.addEventListener('click', async (e) => {
        const userItem = e.target.closest('.modal-user-item');
        if (userItem) {
            const userId = Number(userItem.dataset.userId);
            const username = userItem.dataset.username;
            const chatId = state.activeChat.id;

            if (!chatId) return;

            // Используем кастомный UI вместо confirm
            const confirmed = await new Promise(resolve => {
                const confirmContainer = document.createElement('div');
                confirmContainer.className = 'fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50';
                confirmContainer.innerHTML = `
                    <div class="bg-white p-6 rounded-lg shadow-xl text-center">
                        <p class="mb-4">Вы уверены, что хотите добавить пользователя "${username}" в этот чат?</p>
                        <button id="confirm-yes" class="bg-blue-500 text-white px-4 py-2 rounded mr-2">Да</button>
                        <button id="confirm-no" class="bg-gray-300 px-4 py-2 rounded">Нет</button>
                    </div>
                `;
                document.body.appendChild(confirmContainer);
                document.getElementById('confirm-yes').onclick = () => {
                    confirmContainer.remove();
                    resolve(true);
                };
                document.getElementById('confirm-no').onclick = () => {
                    confirmContainer.remove();
                    resolve(false);
                };
            });

            if (confirmed) {
                try {
                    await api.addUserToChat(chatId, userId);
                    alert(`Пользователь "${username}" успешно добавлен.`);
                    ui.hideModal();
                } catch(error) {
                    alert(`Не удалось добавить пользователя. Ошибка: ${error.message}`);
                }
            }
        }
    });
}


export function setupAuthPage() {
    cleanupChatPolling();

    const form = document.getElementById('auth-form');
    const isLogin = window.location.pathname === '/login';

    form.addEventListener('submit', async (e) => {
        e.preventDefault();
        const username = e.target.username.value;
        const password = e.target.password.value;
        const errorEl = document.getElementById('auth-error');
        errorEl.textContent = '';
        
        try {
            const action = isLogin ? api.login : api.register;
            const data = await action(username, password);
            auth.saveTokens(data.access_token);
            router.navigateTo('/chats');
        } catch (error) {
            errorEl.textContent = error.message;
        }
    });
}

export async function setupChatPage() {
    cleanupChatPolling();

    document.getElementById('logout-btn').addEventListener('click', auth.logout);
    
    document.getElementById('create-chat-btn').addEventListener('click', async () => {
        const chatName = prompt("Введите название нового чата:");
        if (chatName && chatName.trim()) {
            try {
                // Получаем ответ от сервера
                const serverResponse = await api.createChat(chatName.trim());
                
                // --- ИСПРАВЛЕНИЕ ЗДЕСЬ ---
                // Создаем новый объект в формате, который ожидает наш фронтенд,
                // сопоставляя 'chat_id' с 'id' и 'chat_name' с 'name'.
                const newChat = {
                    id: serverResponse.chat_id,
                    name: serverResponse.chat_name
                };

                // Добавляем в состояние уже правильный объект
                state.chats.push(newChat);
                // Обновляем список чатов в UI
                ui.renderChatsList(state.chats);
            } catch (error) {
                alert(`Ошибка создания чата: ${error.message}`);
            }
        }
    });

    const chatView = document.getElementById('active-chat-view');
    if (chatView) {
        chatView.addEventListener('click', (e) => {
            const addUserBtn = e.target.closest('#add-user-to-chat-btn');
            if (addUserBtn) {
                ui.showModal(ui.getAddUserModalTemplate());
                document.getElementById('modal-close-btn').addEventListener('click', ui.hideModal);
                document.getElementById('modal-overlay').addEventListener('click', (ev) => {
                    if (ev.target.id === 'modal-overlay') {
                        ui.hideModal();
                    }
                });
                setupAddUserModal();
            }
        });
    }

    document.getElementById('chats-list').addEventListener('click', async (e) => {
        const chatItem = e.target.closest('.chat-item');
        if (chatItem) {
            const chatId = Number(chatItem.dataset.chatId);
            const { chatName } = chatItem.dataset;

            if (state.activeChat.id === chatId) return;

            cleanupChatPolling();

            state.activeChat.id = chatId;
            state.activeChat.name = chatName;
            ui.setActiveChat(chatId, chatName);

            try {
                const data = await api.getMessages(chatId);
                state.activeChat.messages = data.messages || [];
                ui.renderMessages(state.activeChat.messages);

                console.log(`Starting polling for chat ID: ${chatId}`);
                messagePollingInterval = setInterval(async () => {
                    try {
                        if (state.activeChat.id !== chatId) {
                            cleanupChatPolling();
                            return;
                        }
                        const latestData = await api.getMessages(chatId);
                        const newMessages = latestData.messages || [];
                        if (newMessages.length !== state.activeChat.messages.length) {
                            console.log('New messages found! Updating UI.');
                            state.activeChat.messages = newMessages;
                            ui.renderMessages(state.activeChat.messages);
                        }
                    } catch (error) {
                        console.error('Error during message polling, will try again on next interval:', error);
                    }
                }, 3000);

            } catch (error) {
                console.error('Не удалось загрузить сообщения:', error);
            }
        }
    });
    
    document.getElementById('message-form').addEventListener('submit', async (e) => {
        e.preventDefault();
        const input = document.getElementById('message-input');
        const text = input.value.trim();
        
        if (text && state.activeChat.id) {
            input.value = '';
            try {
                await api.sendMessage(state.activeChat.id, text);
                const data = await api.getMessages(state.activeChat.id);
                state.activeChat.messages = data.messages || [];
                ui.renderMessages(state.activeChat.messages);
            } catch (error) {
                console.error('Не удалось отправить сообщение:', error);
                input.value = text;
                alert('Не удалось отправить сообщение.');
            }
        }
    });

    try {
        const data = await api.getChats();
        state.chats = data.chats || [];
        ui.renderChatsList(state.chats);
    } catch (error) {
        console.error('Не удалось загрузить чаты:', error);
    }
}
