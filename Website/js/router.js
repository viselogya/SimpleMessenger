/**
 * js/router.js
 * * A simple client-side router.
 * It maps URL paths to specific page-rendering and setup functions.
 */
import { auth } from './auth.js';
import { ui } from './ui.js';
// --- ИСПРАВЛЕНИЕ ---
// Добавлен импорт необходимых функций-контроллеров
import { setupAuthPage, setupChatPage } from './controllers.js';

// Объект routes теперь может корректно найти все функции
const routes = {
    '/': '/chats',
    '/login': { render: ui.getAuthFormTemplate.bind(ui, true), setup: setupAuthPage },
    '/register': { render: ui.getAuthFormTemplate.bind(ui, false), setup: setupAuthPage },
    '/chats': { render: ui.getChatLayoutTemplate, setup: setupChatPage, requiresAuth: true },
};

export const router = {
    navigateTo(path) {
        window.history.pushState({}, path, window.location.origin + path);
        this.handleLocation();
    },

    async handleLocation() {
        const path = window.location.pathname;
        let targetPath = path;

        if (routes[path] === '/chats') {
            targetPath = '/chats';
        }

        let route = routes[targetPath];
        
        if (!route) {
            console.warn(`Route not found for ${path}. Redirecting to /login.`);
            this.navigateTo('/login');
            return;
        }

        if (route.requiresAuth && !auth.isLoggedIn()) {
            this.navigateTo('/login');
            return;
        }
        
        if ((targetPath === '/login' || targetPath === '/register') && auth.isLoggedIn()) {
            this.navigateTo('/chats');
            return;
        }

        ui.render(route.render());
        if (route.setup) {
            await route.setup();
        }
    }
};
