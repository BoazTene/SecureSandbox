const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('electronAPI', {
    listRules: () => ipcRenderer.invoke('list-rules'),
    deleteRule: (id) => ipcRenderer.invoke('delete-rule', id),
    addRule: (rule) => ipcRenderer.invoke('add-rule', rule),
    onConnect: (callback) => ipcRenderer.on('is-connected', (event, isConnected) => callback(isConnected)),
    onBlockedPacket: (callback) => ipcRenderer.on("blocked_packet", (event, packet) => callback(packet)),
    removeBlockedPacket: () => ipcRenderer.removeAllListeners("blocked_packet"),
    removeConnect: () => ipcRenderer.removeAllListeners('is-connected'), 
    isConnected: () => ipcRenderer.sendSync('is-connected')
});
