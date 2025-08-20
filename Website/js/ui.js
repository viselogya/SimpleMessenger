/**
 * js/ui.js
 * * Manages the User Interface.
 * Contains functions for rendering HTML templates and updating parts of the DOM.
 */
import { state } from './state.js';

const appRoot = document.getElementById('app-root');

export const ui = {
    // --- Main render function ---
    render(html) {
        appRoot.innerHTML = html;
    },

    // --- Page Templates ---
    getAuthFormTemplate(isLogin = true) {
        const title = isLogin ? 'Вход в аккаунт' : 'Создание аккаунта';
        const submitText = isLogin ? 'Войти' : 'Зарегистрироваться';
        const switchText = isLogin ? 'Нет аккаунта? Зарегистрироваться' : 'Уже есть аккаунт? Войти';
        const switchPath = isLogin ? '/register' : '/login';

        return `
            <div class="flex items-center justify-center min-h-screen bg-gray-100">
                <div class="w-full max-w-md p-8 space-y-6 bg-white rounded-xl shadow-lg">
                    <h2 class="text-3xl font-bold text-center text-gray-900">${title}</h2>
                    <p id="auth-error" class="text-center text-red-500 text-sm h-4"></p>
                    <form id="auth-form" class="space-y-6">
                        <div>
                            <input id="username-input" name="username" type="text" required class="w-full px-4 py-3 text-gray-700 bg-gray-50 border border-gray-300 rounded-lg focus:ring-blue-500 focus:border-blue-500" placeholder="Логин">
                        </div>
                        <div>
                            <input id="password-input" name="password" type="password" required class="w-full px-4 py-3 text-gray-700 bg-gray-50 border border-gray-300 rounded-lg focus:ring-blue-500 focus:border-blue-500" placeholder="Пароль">
                        </div>
                        <div>
                            <button type="submit" class="w-full px-4 py-3 font-semibold text-white bg-blue-600 rounded-lg hover:bg-blue-700 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-blue-500 transition duration-300">
                                ${submitText}
                            </button>
                        </div>
                    </form>
                    <p class="text-sm text-center text-gray-600">
                        <a href="${switchPath}" class="font-medium text-blue-600 hover:underline router-link">${switchText}</a>
                    </p>
                </div>
            </div>
        `;
    },

    getChatLayoutTemplate() {
    return `
      <div class="flex h-full w-full overflow-hidden bg-white">
        <!-- Панель Чатов -->
        <aside id="chats-panel" class="flex flex-col h-full w-1/3 min-w-[300px] bg-gray-50 border-r border-gray-200">
          <header class="flex items-center justify-between p-4 border-b border-gray-200 flex-shrink-0">
            <h2 class="text-xl font-bold text-gray-800">Чаты</h2>
            <div class="flex items-center">
              <!-- Создать чат -->
              <button id="create-chat-btn" title="Создать чат" class="p-2 rounded-full text-gray-500 hover:bg-gray-200 transition">
                <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none"
                     viewBox="0 0 24 24" stroke="currentColor" stroke-width="2">
                  <path stroke-linecap="round" stroke-linejoin="round" d="M12 6v6m0 0v6m0-6h6m-6 0H6" />
                </svg>
              </button>
              <!-- Выйти -->
              <button id="logout-btn" title="Выйти" class="p-2 ml-2 rounded-full text-gray-500 hover:bg-gray-200 transition">
                <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none"
                     viewBox="0 0 24 24" stroke="currentColor" stroke-width="2">
                  <path stroke-linecap="round" stroke-linejoin="round"
                        d="M17 16l4-4m0 0l-4-4m4 4H7m6 4v1a3 3 0 01-3 3H6a3 3 0 01-3-3V7a3 3 0 013-3h4a3 3 0 013 3v1" />
                </svg>
              </button>
            </div>
          </header>
          <div id="chats-list" class="flex-grow overflow-y-auto custom-scrollbar"></div>
        </aside>

        <!-- Панель Сообщений -->
        <main id="messages-panel" class="flex flex-col h-full w-2/3">
          <div id="no-chat-selected" class="flex h-full flex-col items-center justify-center text-gray-500">
            <svg xmlns="http://www.w3.org/2000/svg" class="h-24 w-24 mb-4 text-gray-300" fill="none"
                 viewBox="0 0 24 24" stroke="currentColor" stroke-width="1">
              <path stroke-linecap="round" stroke-linejoin="round"
                    d="M8 12h.01M12 12h.01M16 12h.01M21 12c0 4.418-4.03 8-9 8a9.863
                       9.863 0 01-4.255-.949L3 20l1.395-3.72C3.512 15.042 3
                       13.574 3 12c0-4.418 4.03-8 9-8s9 3.582 9 8z" />
            </svg>
            <h3 class="text-xl">Выберите чат</h3>
            <p class="text-sm">...или создайте новый, чтобы начать общение.</p>
          </div>
          <div id="active-chat-view" class="hidden flex flex-col h-full">
            <header class="flex items-center justify-between p-4 border-b border-gray-200 bg-white flex-shrink-0">
              <h3 id="active-chat-name" class="text-lg font-semibold text-gray-800"></h3>
              <button id="add-user-to-chat-btn" title="Добавить пользователя в чат"
                      class="p-2 rounded-full text-gray-500 hover:bg-gray-200 transition">
                <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none"
                     viewBox="0 0 24 24" stroke="currentColor" stroke-width="2">
                  <path stroke-linecap="round" stroke-linejoin="round"
                        d="M18 9v3m0 0v3m0-3h3m-3 0h-3
                           m-2-5a4 4 0 11-8 0 4 4 0 018 0z
                           M3 20a6 6 0 0112 0v1H3v-1z" />
                </svg>
              </button>
            </header>
            <div id="messages-container"
                 class="flex-grow p-6 overflow-y-auto custom-scrollbar flex flex-col space-y-4"></div>
            <footer class="p-4 bg-gray-50 border-t border-gray-200 flex-shrink-0">
              <form id="message-form" class="flex items-center w-full">
                <input id="message-input" type="text" placeholder="Напишите сообщение..." autocomplete="off"
                       class="w-full px-4 py-2 bg-white border border-gray-300 rounded-full focus:outline-none focus:ring-2 focus:ring-blue-500">
                <button type="submit" class="ml-3 p-2.5 bg-blue-600 text-white rounded-full hover:bg-blue-700 transition">
                  <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none"
                       viewBox="0 0 24 24" stroke="currentColor" stroke-width="2">
                    <path stroke-linecap="round" stroke-linejoin="round"
                          d="M5 13l4 4L19 7" />
                  </svg>
                </button>
              </form>
            </footer>
          </div>
        </main>
      </div>
    `;
  },

    // --- Функции для модального окна ---
    showModal(modalHtml) {
        const modalElement = document.createElement('div');
        modalElement.id = 'modal-container';
        modalElement.innerHTML = modalHtml;
        document.body.appendChild(modalElement);
    },

    hideModal() {
        const modalContainer = document.getElementById('modal-container');
        if (modalContainer) {
            modalContainer.remove();
        }
    },

    getAddUserModalTemplate() {
        return `
            <div id="modal-overlay" class="fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50">
                <div class="bg-white rounded-lg shadow-xl w-full max-w-md max-h-[80vh] flex flex-col">
                    <header class="p-4 border-b flex justify-between items-center">
                        <h3 class="text-lg font-semibold">Добавить пользователя в чат</h3>
                        <button id="modal-close-btn" class="p-1 rounded-full hover:bg-gray-200">
                            <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor" stroke-width="2"><path stroke-linecap="round" stroke-linejoin="round" d="M6 18L18 6M6 6l12 12" /></svg>
                        </button>
                    </header>
                    <div class="p-4">
                        <input id="user-search-input" type="text" placeholder="Поиск по имени..." class="w-full px-3 py-2 border rounded-md">
                    </div>
                    <div id="modal-user-list" class="overflow-y-auto custom-scrollbar p-4 pt-0">
                        </div>
                </div>
            </div>
        `;
    },
    
    renderUserListInModal(users, filter = '') {
        const listEl = document.getElementById('modal-user-list');
        if (!listEl) return;

        const lowerCaseFilter = filter.toLowerCase();
        const filteredUsers = users.filter(user => user.username.toLowerCase().includes(lowerCaseFilter));

        if(filteredUsers.length === 0) {
            listEl.innerHTML = `<p class="text-gray-500 text-center">Пользователи не найдены.</p>`;
            return;
        }

        listEl.innerHTML = filteredUsers.map(user => {
            if (state.currentUser && user.user_id === state.currentUser.id) {
                return '';
            }
            return `
                <div class="modal-user-item flex items-center p-2 rounded-md hover:bg-gray-100 cursor-pointer" data-user-id="${user.user_id}" data-username="${user.username}">
                    <div class="w-10 h-10 rounded-full bg-blue-500 text-white flex items-center justify-center font-bold mr-3">${user.username.charAt(0).toUpperCase()}</div>
                    <span class="font-medium">${user.username}</span>
                </div>
            `;
        }).join('');
    },

    renderChatsList(chats) {
        const listEl = document.getElementById('chats-list');
        if (!listEl) return;
        listEl.innerHTML = chats.map(chat => `
            <div class="chat-item p-4 border-b border-gray-200 cursor-pointer hover:bg-gray-100 transition" data-chat-id="${chat.id}" data-chat-name="${chat.name}">
                <p class="font-semibold text-gray-700 pointer-events-none">${chat.name}</p>
            </div>
        `).join('');
    },
    
    renderMessages(messages) {
        const containerEl = document.getElementById('messages-container');
        if (!containerEl) return;
        
        containerEl.innerHTML = messages.map(msg => {
            const isMine = state.currentUser ? msg.user_id === state.currentUser.id : false;

            const alignClass = isMine ? 'self-end' : 'self-start';
            const bgClass = isMine ? 'bg-blue-500 text-white' : 'bg-gray-200 text-gray-800';
            const usernameHtml = !isMine ? `<div class="font-bold text-sm mb-1 text-blue-700">${msg.username}</div>` : '';

            return `
                <div class="message-bubble flex flex-col ${alignClass} max-w-xs md:max-w-md">
                    <div class="px-4 py-2 rounded-2xl ${bgClass}">
                        ${usernameHtml}
                        <p class="text-sm break-words">${msg.text}</p>
                        <div class="text-xs mt-1 ${isMine ? 'text-blue-200' : 'text-gray-500'} text-right">${new Date(msg.time).toLocaleTimeString('ru-RU', {hour: '2-digit', minute:'2-digit'})}</div>
                    </div>
                </div>
            `;
        }).join('');
        containerEl.scrollTop = containerEl.scrollHeight;
    },
    
    setActiveChat(chatId, chatName) {
        document.getElementById('no-chat-selected').classList.add('hidden');
        const activeChatView = document.getElementById('active-chat-view');
        activeChatView.classList.remove('hidden');
        activeChatView.classList.add('flex');
        
        document.getElementById('active-chat-name').textContent = chatName;

        document.querySelectorAll('.chat-item').forEach(item => {
            item.classList.toggle('bg-blue-100', item.dataset.chatId === String(chatId));
        });
    }
};
