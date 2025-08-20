/**
 * js/state.js
 * * Exports the shared state object for the application.
 * This object holds all the dynamic data that our application needs to function.
 * Centralizing the state makes it easier to manage and debug.
 */

export const state = {
    currentUser: null, // Holds { id, username } of the logged-in user
    accessToken: null,
    refreshToken: null,
    chats: [], // Array of chat objects: [{ id, name }]
    activeChat: {
        id: null,
        name: '',
        messages: [] // Array of message objects: [{ userId, username, text, time }]
    },
    // --- НОВОЕ ---
    // Хранилище для списка всех пользователей, чтобы не запрашивать их каждый раз.
    allUsers: [],
};
