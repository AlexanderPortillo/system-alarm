let port;
let reader;
let decoder;
let inputText;

const connectButton = document.getElementById("connectButton");
const dataDiv = document.getElementById("data");
const statusDiv = document.getElementById("status");
const fireStatusDiv = document.getElementById("fireStatus");
const smokeStatusDiv = document.getElementById("smokeStatus");
const names = ["Denis Martel", "Jesse Miranda", "Kelly Garcia"];

// Función para conectarse al puerto serial
connectButton.addEventListener("click", async () => {
    try {
        // Solicitar acceso al puerto serial
        port = await navigator.serial.requestPort();
        await port.open({ baudRate: 9600 });

        statusDiv.textContent = "Conectado al Arduino. Leyendo datos...";

        // Crear un lector para el puerto serial
        reader = port.readable.getReader();
        decoder = new TextDecoder();
        readLoop(reader); // Iniciar el bucle de lectura
    } catch (err) {
        statusDiv.textContent = "Error al conectar al puerto serial: " + err;
    }
});

// Función para leer los datos del puerto serial
async function readLoop(reader) {
    while (true) {
        try {
            const { value, done } = await reader.read();
            if (done) {
                break;
            }
            // Decodificar los datos y actualizar el HTML
            inputText = decoder.decode(value, { stream: true });

            // Mostrar los datos en la página
            dataDiv.textContent = "Estado de los Sensores: " + inputText;

            // Verificar los datos recibidos y mostrar mensajes específicos
            if (inputText.includes("Fuego detectado")) {
                // Mostrar alerta de fuego
                fireStatusDiv.style.display = "block";
                smokeStatusDiv.style.display = "none";  // Ocultar mensaje de humo
            } else if (inputText.includes("Humo detectado")) {
                // Mostrar alerta de humo
                smokeStatusDiv.style.display = "block";
                fireStatusDiv.style.display = "none";  // Ocultar mensaje de fuego
            } else {
                // Si no hay fuego ni humo, ocultamos ambos
                fireStatusDiv.style.display = "none";
                smokeStatusDiv.style.display = "none";
            }

        } catch (err) {
            statusDiv.textContent = "Error de lectura: " + err;
        }
    }
}

// Función para mostrar tres nombres aleatorios
function showRandomNames() {
    const randomNamesDiv = document.getElementById('randomNames');
    randomNamesDiv.innerHTML = '';  // Limpiar cualquier contenido previo

    // Seleccionar tres nombres aleatorios
    const randomSelectedNames = [];
    while (randomSelectedNames.length < 3) {
        const randomName = names[Math.floor(Math.random() * names.length)];
        if (!randomSelectedNames.includes(randomName)) {
            randomSelectedNames.push(randomName);
        }
    }

    // Crear un elemento para cada nombre y añadirlo al DOM
    randomSelectedNames.forEach(name => {
        const nameElement = document.createElement('div');
        nameElement.classList.add('name');
        nameElement.textContent = name;
        randomNamesDiv.appendChild(nameElement);
    });
}

// Mostrar nombres aleatorios cada 2 segundos
setInterval(showRandomNames, 2000);

// Llamada inicial para mostrar los nombres inmediatamente
showRandomNames();