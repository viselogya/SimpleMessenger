/**
 * js/api.js
 * * Manages all communication with the backend server.
 */

import { state } from './state.js';
import { auth } from './auth.js';

const API_BASE_URL = 'https://localhost:443';

async function _fetch(endpoint, options = {}) {
    const headers = { ...options.headers };

    if (state.accessToken) {
        headers['Authorization'] = `Bearer ${state.accessToken}`;
    }

    try {
        const response = await fetch(`${API_BASE_URL}${endpoint}`, { ...options, headers });

        if (response.status === 204 || response.headers.get("content-length") === null || response.headers.get("content-length") === "0") {
            return null;
        }
        
        const data = await response.json();

        if (!response.ok) {
            // --- ИЗМЕНЕНИЕ ---
            // Теперь мы не проверяем наличие refreshToken в state, так как он в cookie.
            // Просто пытаемся обновиться, если получили 401.
            if (response.status === 401) {
                console.log('Access token expired, attempting to refresh...');
                const refreshed = await api.refreshToken();
                if (refreshed) {
                    console.log('Token refreshed successfully. Retrying original request.');
                    headers['Authorization'] = `Bearer ${state.accessToken}`;
                    const retryResponse = await fetch(`${API_BASE_URL}${endpoint}`, { ...options, headers });
                    const retryData = await retryResponse.json();
                    if (!retryResponse.ok) throw new Error(retryData.detail || 'Error after token refresh');
                    return retryData;
                }
            }
            throw new Error(data.detail || `Server error: ${response.status}`);
        }
        return data;
    } catch (error) {
        console.error(`API Error on ${endpoint}:`, error);
        throw error;
    }
}

export const api = {
    login: (username, password) => _fetch('/login', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ username, password }),
    }),

    register: (username, password) => _fetch('/registration', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ username, password }),
    }),

    refreshToken: async () => {
        try {
            // --- ИЗМЕНЕНИЕ ---
            // Отправляем запрос без тела. Браузер сам прикрепит HttpOnly cookie.
            const data = await _fetch('/token/refresh', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
            });
            // Сохраняем только новый access_token
            auth.saveTokens(data.access_token);
            return true;
        } catch (error) {
            console.error('Failed to refresh token.', error);
            auth.logout();
            return false;
        }
    },

    createChat: (chat_name) => _fetch('/create_chat', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ chat_name }),
    }),

    getChats: () => _fetch('/chats', { method: 'GET' }),

    getMessages: (chat_id) => _fetch(`/chats/messages?chat_id=${encodeURIComponent(chat_id)}`, {
        method: 'GET',
    }),

    sendMessage: (chat_id, text) => {
        const time = new Date().toISOString().slice(0, 16).replace('T', ' ');
        return _fetch('/chats/messages/send_message', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ chat_id, text, time }),
        });
    },
    
    getUsers: () => _fetch('/users', { method: 'GET'}),

    addUserToChat: (chat_id, addedUser_id) => _fetch('/chats/add_user', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ chat_id, addedUser_id }),
    }),
};
