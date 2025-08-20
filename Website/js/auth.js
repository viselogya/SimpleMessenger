/**
 * js/auth.js
 * * Handles all authentication-related logic.
 * Manages only the access token, as the refresh token is in a secure HttpOnly cookie.
 */

import { state } from './state.js';
import { router } from './router.js';

function decodeJwt(token) {
    try {
        return JSON.parse(atob(token.split('.')[1]));
    } catch (e) {
        console.error("Failed to decode JWT", e);
        return null;
    }
}

export const auth = {
    init() {
        const accessToken = localStorage.getItem('accessToken');
        const user = localStorage.getItem('user');

        if (accessToken && user) {
            state.accessToken = accessToken;
            state.currentUser = JSON.parse(user);
        }
    },

    saveTokens(accessToken) {
        state.accessToken = accessToken;
        localStorage.setItem('accessToken', accessToken);
        
        const decodedToken = decodeJwt(accessToken);
        if (decodedToken && decodedToken.sub && decodedToken.username) {
             state.currentUser = { id: Number(decodedToken.sub), username: decodedToken.username };
             localStorage.setItem('user', JSON.stringify(state.currentUser));
             console.log('User info saved successfully:', state.currentUser);
        } else {
            console.error('Could not decode user info from token OR token is missing required fields (sub, username).', decodedToken);
            state.currentUser = null;
            localStorage.removeItem('user');
        }
    },

    logout() {
        // Очищаем состояние пользователя
        state.accessToken = null;
        state.currentUser = null;

        // --- ИСПРАВЛЕНИЕ ЗДЕСЬ ---
        // Сбрасываем "призрачный" активный чат до начального состояния.
        // Это позволит новому пользователю без проблем выбирать любой чат.
        state.activeChat = { id: null, name: '', messages: [] };
        
        // Очищаем localStorage
        localStorage.removeItem('accessToken');
        localStorage.removeItem('user');
        
        // Переходим на страницу входа
        router.navigateTo('/login');
    },

    isLoggedIn() {
        return !!state.accessToken;
    },
};
