import { auth } from './auth.js';
import { router } from './router.js';


window.addEventListener('click', e => {
    // e.target.closest() allows clicking on elements inside the link
    const link = e.target.closest('.router-link');
    if (link) {
        e.preventDefault();
        router.navigateTo(link.getAttribute('href'));
    }
});

window.addEventListener('popstate', () => router.handleLocation());

function initializeApp() {
    // Check if a user session exists in localStorage
    auth.init();
    // Handle the initial URL to render the correct page on load
    router.handleLocation();
}

// Start the app
initializeApp();