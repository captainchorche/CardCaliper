// CardCaliper Web Application
class CardCaliperApp {
    constructor() {
        this.ws = null;
        this.reconnectAttempts = 0;
        this.maxReconnectAttempts = 5;
        this.reconnectDelay = 1000;
        this.savedMeasurements = [];
        this.currentMeasurement = '---.--';

        // Touch and mouse tracking for swipe/drag to delete
        this.touchStartX = null;
        this.touchStartY = null;
        this.touchStartTime = null;
        this.isSwiping = false;
        this.swipeElement = null;
        this.mouseStartX = null;
        this.mouseStartY = null;
        this.mouseStartTime = null;
        this.isDragging = false;
        this.dragElement = null;
        this.originalTransform = '';

        this.elements = {
            measurement: document.getElementById('measurement'),
            status: document.getElementById('status'),
            measurementsTable: document.getElementById('measurementsTable')
        };

        // Bind methods for global event listeners
        this.boundHandleMouseMove = this.handleMouseMove.bind(this);
        this.boundHandleMouseUp = this.handleMouseUp.bind(this);

        this.init();
    }
    
    init() {
        this.loadSavedMeasurements();
        this.connectWebSocket();
        this.setupEventListeners();
        this.updateMeasurementsDisplay();
    }
    
    connectWebSocket() {
        const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
        const wsUrl = `${protocol}//${window.location.host}/ws`;
        
        try {
            this.ws = new WebSocket(wsUrl);
            this.setupWebSocketEvents();
        } catch (error) {
            console.error('WebSocket connection failed:', error);
            this.handleConnectionError();
        }
    }
    
    setupWebSocketEvents() {
        this.ws.onopen = () => {
            console.log('WebSocket connected');
            this.updateStatus('Connected', 'connected');
            this.reconnectAttempts = 0;
        };
        
        this.ws.onclose = () => {
            console.log('WebSocket disconnected');
            this.updateStatus('Disconnected', 'offline');
            this.scheduleReconnect();
        };
        
        this.ws.onerror = (error) => {
            console.error('WebSocket error:', error);
            this.updateStatus('Connection Error', 'offline');
        };
        
        this.ws.onmessage = (event) => {
            try {
                const data = JSON.parse(event.data);
                this.updateMeasurement(data.measurement);
            } catch (error) {
                console.error('Failed to parse WebSocket message:', error);
            }
        };
    }
    
    updateStatus(text, className = '') {
        this.elements.status.textContent = text;
        this.elements.status.className = `status ${className}`;
    }
    
    updateMeasurement(value) {
        this.currentMeasurement = value;
        this.elements.measurement.textContent = value;

        // Add visual feedback for new measurements
        this.elements.measurement.style.transform = 'scale(1.05)';
        setTimeout(() => {
            this.elements.measurement.style.transform = 'scale(1)';
        }, 150);
    }



    // Copy measurement value to clipboard
    async copyToClipboard(value, element) {
        try {
            // Try modern clipboard API first
            if (navigator.clipboard && window.isSecureContext) {
                await navigator.clipboard.writeText(value);
                this.showCopyFeedback(element, 'Copied!');
            } else {
                // Fallback for older browsers or non-HTTPS
                const textArea = document.createElement('textarea');
                textArea.value = value;
                textArea.style.position = 'fixed';
                textArea.style.left = '-999999px';
                textArea.style.top = '-999999px';
                document.body.appendChild(textArea);
                textArea.focus();
                textArea.select();

                if (document.execCommand('copy')) {
                    this.showCopyFeedback(element, 'Copied!');
                } else {
                    this.showCopyFeedback(element, 'Copy failed', true);
                }

                document.body.removeChild(textArea);
            }
        } catch (error) {
            console.error('Failed to copy to clipboard:', error);
            this.showCopyFeedback(element, 'Copy failed', true);
        }
    }

    // Show visual feedback for copy operation
    showCopyFeedback(element, message, isError = false) {
        // Store original content
        const originalContent = element.innerHTML;
        const originalBackground = element.style.background;

        // Show feedback
        element.style.background = isError ? 'rgba(239, 68, 68, 0.2)' : 'rgba(16, 185, 129, 0.2)';
        element.style.transition = 'background 0.2s ease';

        // Create temporary feedback element
        const feedback = document.createElement('div');
        feedback.style.cssText = `
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            background: ${isError ? 'var(--error)' : 'var(--success)'};
            color: white;
            padding: 4px 8px;
            border-radius: 4px;
            font-size: 0.75rem;
            font-weight: 500;
            z-index: 1000;
            pointer-events: none;
        `;
        feedback.textContent = message;

        // Position relative to element
        element.style.position = 'relative';
        element.appendChild(feedback);

        // Remove feedback after delay
        setTimeout(() => {
            element.style.background = originalBackground;
            if (feedback.parentNode) {
                feedback.parentNode.removeChild(feedback);
            }
        }, 1500);
    }

    // Touch handling for swipe to delete
    handleTouchStart(event, element) {
        this.touchStartX = event.touches[0].clientX;
        this.touchStartY = event.touches[0].clientY;
        this.touchStartTime = Date.now();
        this.isSwiping = false;
        this.swipeElement = element;

        // Store original transform
        this.originalTransform = element.style.transform || '';
    }

    handleTouchMove(event, element) {
        if (!this.touchStartX || !this.touchStartY) return;

        const touchX = event.touches[0].clientX;
        const touchY = event.touches[0].clientY;
        const deltaX = touchX - this.touchStartX;
        const deltaY = touchY - this.touchStartY;

        // Check if this is a horizontal swipe
        if (Math.abs(deltaX) > Math.abs(deltaY) && Math.abs(deltaX) > 15) {
            this.isSwiping = true;
            // Only prevent scrolling if we're clearly swiping horizontally
            if (Math.abs(deltaX) > 25) {
                event.preventDefault();
            }

            // Apply transform with resistance
            const resistance = 0.6;
            const translateX = deltaX * resistance;

            // Add visual feedback based on swipe direction and distance
            let backgroundColor = 'var(--surface-elevated)';
            if (Math.abs(deltaX) > 50) {
                backgroundColor = 'rgba(239, 68, 68, 0.3)'; // Red for delete
            }

            element.style.transform = `translateX(${translateX}px)`;
            element.style.background = backgroundColor;
            element.style.transition = 'none';
        }
    }

    handleTouchEnd(event, element) {
        if (!this.touchStartX || !this.swipeElement) return;

        const touchEndTime = Date.now();
        const touchDuration = touchEndTime - this.touchStartTime;

        // Calculate final delta
        const finalX = event.changedTouches[0].clientX;
        const deltaX = finalX - this.touchStartX;

        // Reset transition
        element.style.transition = 'all 0.3s cubic-bezier(0.4, 0, 0.2, 1)';

        // Check if this was a swipe to delete
        if (this.isSwiping && Math.abs(deltaX) > 80 && touchDuration < 500) {
            // Swipe to delete
            this.deleteMeasurement(element, parseInt(element.dataset.index));
        } else if (this.isSwiping) {
            // Reset position
            element.style.transform = this.originalTransform;
            element.style.background = 'var(--surface-elevated)';
        } else if (!this.isSwiping && touchDuration < 300) {
            // This was a tap, not a swipe - trigger copy
            const value = element.querySelector('.measurement-value').textContent;
            this.copyToClipboard(value, element);
        }

        // Reset touch tracking
        this.touchStartX = null;
        this.touchStartY = null;
        this.touchStartTime = null;
        this.isSwiping = false;
        this.swipeElement = null;
    }

    // Delete measurement with animation
    deleteMeasurement(element, index) {
        // Animate deletion
        element.style.transform = 'translateX(100%)';
        element.style.opacity = '0';
        element.style.background = 'rgba(239, 68, 68, 0.5)';

        setTimeout(() => {
            // Remove from array
            this.savedMeasurements.splice(index, 1);

            // Save to localStorage
            this.saveMeasurementsToStorage();

            // Update display
            this.updateMeasurementsDisplay();

            // Show feedback
            this.showDeleteFeedback();
        }, 300);
    }

    // Show delete feedback
    showDeleteFeedback() {
        const container = this.elements.measurementsTable.parentElement;

        const feedback = document.createElement('div');
        feedback.style.cssText = `
            position: absolute;
            top: 20px;
            right: 20px;
            background: var(--error);
            color: white;
            padding: 8px 16px;
            border-radius: 8px;
            font-size: 0.875rem;
            font-weight: 500;
            z-index: 1000;
            pointer-events: none;
            transform: translateY(-20px);
            opacity: 0;
            transition: all 0.3s ease;
        `;
        feedback.textContent = '🗑️ Measurement deleted';

        container.style.position = 'relative';
        container.appendChild(feedback);

        // Animate in
        setTimeout(() => {
            feedback.style.transform = 'translateY(0)';
            feedback.style.opacity = '1';
        }, 10);

        // Remove after delay
        setTimeout(() => {
            feedback.style.transform = 'translateY(-20px)';
            feedback.style.opacity = '0';
            setTimeout(() => {
                if (feedback.parentNode) {
                    feedback.parentNode.removeChild(feedback);
                }
            }, 300);
        }, 2000);
    }

    // Handle click events (prevent during drag)
    handleClick(event, element, value) {
        // Don't trigger click if this was a drag operation
        if (this.isDragging) {
            event.preventDefault();
            return;
        }
        this.copyToClipboard(value, element);
    }

    // Mouse handling for desktop drag to delete
    handleMouseDown(event, element) {
        // Only handle left mouse button
        if (event.button !== 0) return;

        this.mouseStartX = event.clientX;
        this.mouseStartY = event.clientY;
        this.mouseStartTime = Date.now();
        this.isDragging = false;
        this.dragElement = element;

        // Store original transform
        this.originalTransform = element.style.transform || '';

        // Prevent text selection during drag
        event.preventDefault();
        document.addEventListener('mousemove', this.boundHandleMouseMove);
        document.addEventListener('mouseup', this.boundHandleMouseUp);
    }

    handleMouseMove(event, element) {
        if (!this.mouseStartX || !this.dragElement) return;

        const mouseX = event.clientX;
        const mouseY = event.clientY;
        const deltaX = mouseX - this.mouseStartX;
        const deltaY = mouseY - this.mouseStartY;

        // Check if this is a horizontal drag
        if (Math.abs(deltaX) > Math.abs(deltaY) && Math.abs(deltaX) > 5) {
            this.isDragging = true;

            // Apply transform with resistance
            const resistance = 0.6;
            const translateX = deltaX * resistance;

            // Add visual feedback based on drag direction and distance
            let backgroundColor = 'var(--surface-elevated)';
            if (Math.abs(deltaX) > 50) {
                backgroundColor = 'rgba(239, 68, 68, 0.3)'; // Red for delete
            }

            this.dragElement.style.transform = `translateX(${translateX}px)`;
            this.dragElement.style.background = backgroundColor;
            this.dragElement.style.transition = 'none';
            this.dragElement.style.cursor = 'grabbing';
        }
    }

    handleMouseUp(event, element) {
        if (!this.mouseStartX || !this.dragElement) return;

        const mouseEndTime = Date.now();
        const mouseDuration = mouseEndTime - this.mouseStartTime;

        // Calculate final delta
        const finalX = event.clientX;
        const deltaX = finalX - this.mouseStartX;

        // Reset transition and cursor
        this.dragElement.style.transition = 'all 0.3s cubic-bezier(0.4, 0, 0.2, 1)';
        this.dragElement.style.cursor = 'pointer';

        // Check if this was a drag to delete
        if (this.isDragging && Math.abs(deltaX) > 80 && mouseDuration < 1000) {
            // Drag to delete
            this.deleteMeasurement(this.dragElement, parseInt(this.dragElement.dataset.index));
        } else if (this.isDragging) {
            // Reset position
            this.dragElement.style.transform = this.originalTransform;
            this.dragElement.style.background = 'var(--surface-elevated)';
        }

        // Reset drag tracking
        this.mouseStartX = null;
        this.mouseStartY = null;
        this.mouseStartTime = null;
        this.dragElement = null;

        // Reset isDragging after a short delay to prevent click
        setTimeout(() => {
            this.isDragging = false;
        }, 10);

        // Remove global event listeners
        document.removeEventListener('mousemove', this.boundHandleMouseMove);
        document.removeEventListener('mouseup', this.boundHandleMouseUp);
    }

    handleMouseLeave(event, element) {
        // Reset if mouse leaves the element during drag
        if (this.isDragging && this.dragElement === element) {
            element.style.transform = this.originalTransform;
            element.style.background = 'var(--surface-elevated)';
            element.style.transition = 'all 0.3s cubic-bezier(0.4, 0, 0.2, 1)';
            element.style.cursor = 'pointer';
        }
    }
    
    scheduleReconnect() {
        if (this.reconnectAttempts < this.maxReconnectAttempts) {
            this.reconnectAttempts++;
            this.updateStatus(`Reconnecting... (${this.reconnectAttempts}/${this.maxReconnectAttempts})`, 'connecting');
            
            setTimeout(() => {
                this.connectWebSocket();
            }, this.reconnectDelay * this.reconnectAttempts);
        } else {
            this.updateStatus('Connection Failed', 'offline');
        }
    }
    
    handleConnectionError() {
        this.updateStatus('Connection Failed', 'offline');
    }
    

    
    setupEventListeners() {
        // Handle page visibility changes
        document.addEventListener('visibilitychange', () => {
            if (document.visibilityState === 'visible' && this.ws.readyState !== WebSocket.OPEN) {
                this.connectWebSocket();
            }
        });

        // Handle online/offline events
        window.addEventListener('online', () => {
            this.connectWebSocket();
        });

        window.addEventListener('offline', () => {
            this.updateStatus('Offline', 'offline');
        });

        // Handle keyboard events for desktop
        this.setupKeyboardListeners();
    }

    setupKeyboardListeners() {
        // Detect if we're on a desktop device (has keyboard)
        const isDesktop = !('ontouchstart' in window) && window.innerWidth > 768;

        if (isDesktop) {
            document.addEventListener('keydown', (event) => {
                // Save measurement on Enter key press
                if (event.key === 'Enter' && !event.ctrlKey && !event.altKey && !event.shiftKey) {
                    // Prevent default behavior
                    event.preventDefault();

                    // Don't trigger if user is typing in an input field
                    const activeElement = document.activeElement;
                    if (activeElement && (activeElement.tagName === 'INPUT' || activeElement.tagName === 'TEXTAREA')) {
                        return;
                    }

                    // Save current measurement
                    this.addMeasurement(this.currentMeasurement);
                }
            });
        }
    }

    // Measurements management methods
    loadSavedMeasurements() {
        try {
            const saved = localStorage.getItem('cardcaliper_measurements');
            if (saved) {
                this.savedMeasurements = JSON.parse(saved);
            }
        } catch (error) {
            console.error('Failed to load saved measurements:', error);
            this.savedMeasurements = [];
        }
    }

    saveMeasurementsToStorage() {
        try {
            localStorage.setItem('cardcaliper_measurements', JSON.stringify(this.savedMeasurements));
        } catch (error) {
            console.error('Failed to save measurements:', error);
        }
    }

    addMeasurement(value) {
        if (value === '---.--' || value === '---.-' || !value) {
            return false; // Don't save invalid measurements
        }

        const measurement = {
            id: Date.now(),
            value: value,
            timestamp: new Date().toLocaleTimeString()
        };

        this.savedMeasurements.push(measurement);
        this.saveMeasurementsToStorage();
        this.updateMeasurementsDisplay();

        // Visual feedback
        this.showSaveAnimation();

        return true;
    }

    clearAllMeasurements() {
        this.savedMeasurements = [];
        this.saveMeasurementsToStorage();
        this.updateMeasurementsDisplay();
    }

    updateMeasurementsDisplay() {
        const container = this.elements.measurementsTable;

        if (this.savedMeasurements.length === 0) {
            container.innerHTML = `
                <div class="empty-state">
                    <div class="empty-icon">📏</div>
                    <div class="empty-text">No measurements saved yet</div>
                    <div class="empty-hint">Click on measurement to save</div>
                </div>
            `;
            return;
        }

        // Reverse the array to show latest measurements first, but keep original numbering
        const measurementsHtml = this.savedMeasurements
            .slice()  // Create a copy to avoid modifying original array
            .reverse()  // Reverse to show latest first
            .map((measurement, reverseIndex) => {
                // Calculate the original index for proper numbering (1, 2, 3...)
                const originalIndex = this.savedMeasurements.length - reverseIndex;
                const actualIndex = this.savedMeasurements.length - 1 - reverseIndex; // Index in original array
                return `
                    <div class="measurement-row"
                         data-index="${actualIndex}"
                         onclick="caliperApp.handleClick(event, this, '${measurement.value}')"
                         ontouchstart="caliperApp.handleTouchStart(event, this)"
                         ontouchmove="caliperApp.handleTouchMove(event, this)"
                         ontouchend="caliperApp.handleTouchEnd(event, this)"
                         onmousedown="caliperApp.handleMouseDown(event, this)"
                         onmousemove="caliperApp.handleMouseMove(event, this)"
                         onmouseup="caliperApp.handleMouseUp(event, this)"
                         onmouseleave="caliperApp.handleMouseLeave(event, this)">
                        <span class="measurement-number">${originalIndex}.</span>
                        <span class="measurement-value">${measurement.value}</span>
                        <span class="measurement-time">${measurement.timestamp}</span>
                    </div>
                `;
            })
            .join('');

        container.innerHTML = measurementsHtml;
    }

    showSaveAnimation() {
        const measurementDisplay = document.querySelector('.measurement-display');
        measurementDisplay.style.background = 'rgba(7, 224, 0, 0.2)';
        measurementDisplay.style.borderColor = '#07E0';

        setTimeout(() => {
            measurementDisplay.style.background = 'rgba(0, 31, 63, 0.3)';
            measurementDisplay.style.borderColor = 'var(--border-color)';
        }, 300);
    }

    downloadMeasurementsCSV() {
        if (this.savedMeasurements.length === 0) {
            alert('No measurements to download');
            return;
        }

        // Создаем CSV контент
        let csvContent = 'Number,Measurement (mm),Time\n';

        this.savedMeasurements.forEach((measurement, index) => {
            csvContent += `${index + 1},${measurement.value},${measurement.timestamp}\n`;
        });

        // Создаем blob и ссылку для скачивания
        const blob = new Blob([csvContent], { type: 'text/csv;charset=utf-8;' });
        const link = document.createElement('a');

        if (link.download !== undefined) {
            const url = URL.createObjectURL(blob);
            link.setAttribute('href', url);

            // Генерируем имя файла с текущей датой и временем
            const now = new Date();
            const dateStr = now.toISOString().slice(0, 19).replace(/:/g, '-');
            link.setAttribute('download', `CardCaliper_measurements_${dateStr}.csv`);

            link.style.visibility = 'hidden';
            document.body.appendChild(link);
            link.click();
            document.body.removeChild(link);

            // Показываем уведомление об успешном скачивании
            this.showDownloadAnimation();
        }
    }

    showDownloadAnimation() {
        const downloadBtn = document.querySelector('.btn-download');
        const originalText = downloadBtn.textContent;

        downloadBtn.textContent = '✅ Downloaded';
        downloadBtn.style.background = 'rgba(7, 224, 0, 0.4)';

        setTimeout(() => {
            downloadBtn.textContent = originalText;
            downloadBtn.style.background = 'rgba(7, 224, 0, 0.2)';
        }, 2000);
    }
}

// Utility functions
function requestFullscreen() {
    const container = document.querySelector('.container');
    if (container.requestFullscreen) {
        container.requestFullscreen();
    } else if (container.webkitRequestFullscreen) {
        container.webkitRequestFullscreen();
    } else if (container.msRequestFullscreen) {
        container.msRequestFullscreen();
    }
}

function saveMeasurement() {
    if (window.caliperApp) {
        const success = window.caliperApp.addMeasurement(window.caliperApp.currentMeasurement);
        if (!success) {
            // Show error feedback for invalid measurements
            const measurementDisplay = document.querySelector('.measurement-display');
            measurementDisplay.style.background = 'rgba(248, 0, 0, 0.2)';
            measurementDisplay.style.borderColor = '#F800';

            setTimeout(() => {
                measurementDisplay.style.background = 'rgba(0, 31, 63, 0.3)';
                measurementDisplay.style.borderColor = 'var(--border-color)';
            }, 300);
        }
    }
}

function clearMeasurements() {
    if (window.caliperApp && confirm('Are you sure you want to clear all saved measurements?')) {
        window.caliperApp.clearAllMeasurements();
    }
}

function downloadCSV() {
    if (window.caliperApp) {
        window.caliperApp.downloadMeasurementsCSV();
    }
}

// Initialize app when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    window.caliperApp = new CardCaliperApp();
});

// PWA Service Worker registration
if ('serviceWorker' in navigator) {
    window.addEventListener('load', () => {
        navigator.serviceWorker.register('/sw.js')
            .then(registration => {
                console.log('SW registered: ', registration);
            })
            .catch(registrationError => {
                console.log('SW registration failed: ', registrationError);
            });
    });
}
