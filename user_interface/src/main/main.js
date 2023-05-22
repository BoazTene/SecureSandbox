const { app, BrowserWindow } = require("electron");
const path = require("path");
const url = require("url");
const IPCUtlity = require("./IPCUtility.js");

let mainWindow;

function createWindow() {
    mainWindow = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            preload: path.join(__dirname, "../preload.js"),
            nodeIntegration: true,
            enableRemoteModule: true,
            contextIsolation: true,
            webSecurity: false,
            allowFileAccess: true,
        },
    });

    mainWindow.loadURL(
        url.format({
            pathname: path.join(__dirname, "../../public/index.html"),
            protocol: "file:",
            slashes: true,
        })
    );
    mainWindow.on("closed", function () {
        mainWindow = null;
    });

    const ipcUtility = new IPCUtlity();
    ipcUtility.registerHooks(mainWindow);
}

app.on("ready", createWindow);

app.on("window-all-closed", function () {
    if (process.platform !== "darwin") {
        app.quit();
    }
});

app.on("activate", function () {
    if (mainWindow === null) {
        createWindow();
    }
});
