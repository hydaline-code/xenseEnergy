
const icons = {
  solarPower: `<svg xmlns="http://www.w3.org/2000/svg" height="48" viewBox="0 -960 960 960" width="48"><path d="m80-80 80-400h640l80 400H80Zm40-740v-60h120v60H120Zm34 680h296v-110H176l-22 110Zm80-460-43-42 85-85 43 42-85 85Zm-46 290h262v-110H210l-22 110Zm292-390q-74 0-125-52.5T300-880h60q3 50 37 85t83 35q49 0 83-35t37-85h60q-4 75-55 127.5T480-700Zm0-180Zm-30 360v-120h60v120h-60Zm60 380h296l-22-110H510v110Zm0-170h262l-22-110H510v110Zm216-291-85-85 42-42 86 84-43 43Zm-6-219v-60h120v60H720Z"/></svg>`,
  enter: `<svg width="34" height="27" viewBox="0 0 34 27" fill="none" xmlns="http://www.w3.org/2000/svg">
  <path d="M19.0401 0.986284C19.4794 0.547044 20.0753 0.300293 20.6965 0.300293C21.3178 0.300293 21.9136 0.547044 22.353 0.986284L32.8964 11.5297C33.3356 11.969 33.5824 12.5649 33.5824 13.1862C33.5824 13.8074 33.3356 14.4033 32.8964 14.8426L22.353 25.386C21.9111 25.8128 21.3193 26.049 20.705 26.0436C20.0907 26.0383 19.503 25.7919 19.0686 25.3575C18.6342 24.9231 18.3878 24.3354 18.3824 23.7211C18.3771 23.1068 18.6133 22.515 19.0401 22.0731L25.3825 15.5291H3.12423C2.50283 15.5291 1.90688 15.2823 1.46749 14.8429C1.0281 14.4035 0.78125 13.8076 0.78125 13.1862C0.78125 12.5648 1.0281 11.9688 1.46749 11.5294C1.90688 11.09 2.50283 10.8432 3.12423 10.8432H25.3825L19.0401 4.29925C18.6008 3.85988 18.3541 3.26404 18.3541 2.64277C18.3541 2.02149 18.6008 1.42566 19.0401 0.986284Z" fill="#3AA945"/>
  </svg>`
};

const websocketUrl = 'ws://192.168.1.1/ws';
let socket = new WebSocket(websocketUrl);;

/* window.addEventListener('load', function() {
  
});
 */
let serverSettings = { threshold: [] };
let clientSettings = { threshold: [] };

const main = document.querySelector('main');
const header = document.querySelector('header');
const footer = document.querySelector('footer');

let pass = '';
let start = false;
let msg = null;
let sucessSubmit = true;

function clientConnected() {
  if (msg[0] === 2)
  footer.firstElementChild.innerHTML = `Only you and a user are connected to this Power Manager`;
  else if (msg[0] > 2)  
    footer.firstElementChild.innerHTML = `You and ${msg[0] - 1} are connected are connected to Power Manager`;
  else footer.firstElementChild.innerHTML = 'You are connected to your Energy Manager'
}

function createThresholdBtn() {
  const article2 = document.createElement('article');
  article2.setAttribute('id', 'threshold');
  const span = document.createElement('span');
  const span2 = document.createElement('span');
  span.innerHTML = icons.enter;
  span2.innerText = 'Change threshold';
  article2.append(span, span2);
  document.querySelector('main').append(article2);
}

function item1Content() {
  const item1 = document.querySelector('.item1');
  const h2 = document.createElement('h2');
  const article = document.createElement('article');
  const div = document.createElement('div');
  const sunContainer = document.createElement('div');
  sunContainer.className = 'sunContainer';
  
  div.className = 'sun';
  h2.className = 'nrTxt';
  sunContainer.append(div);
  article.className = 'sunH';
  article.append(sunContainer, h2);
  h2.innerHTML = `Sunlight intensity threshold for power activation`;
  item1.append(article);
}

function createHomePage() {
  const div = document.createElement('div');
  div.className = 'sun-reading-container'
  const h2 = Object.assign(document.createElement('h1'), { id: 'sun-reading' });
  h2.textContent ='0';
  const p = document.createElement('p');
  p.className = 'sun-light-summary';
  div.append(h2, p);
  const section = document.createElement('section');
  section.setAttribute('id', 'home');
  const article1 = document.createElement('article');
  article1.setAttribute('class', 'item1 item');
  const article2 = document.createElement('article');
  article2.setAttribute('class', 'item2 item');
  section.append(article2, article1);
  main.append(div, section);  
  document.querySelector('.item2').innerHTML = `Maximum normal surface irradiance is approximately 1000 W/m&#178; at sea level on a clear day`

  item1Content();
  createThresholdBtn();
}


//Loging Page
setTimeout(() => {
  document.querySelector('#logo-anim').style.left = '28px' 
}, 1000);

document.querySelector('#passcode').addEventListener('keyup', (e) => {
  const { target } = e;
  const passLabel = document.querySelector('#pass-lebel');
  if (target.value.length >= 4) {
    if (pass < 1) {
      passLabel.innerHTML = 'Your device is not connected to your Energy Manager. Check your WiFi and refresh page';
      passLabel.style.color = '#f28500';
    }
    else if (target.value !== pass) {
      passLabel.innerHTML = 'Passcode Invalid';
      passLabel.style.color = '#f28500';
    }
    else {
      document.querySelector('body').classList.remove('body');
      document.querySelector('.login-box').remove();
      createHomePage();
      clientConnected();
      document.querySelector('.nrTxt').innerHTML = `Power activation for the connected Energy manager is between: <br> 
      ${serverSettings.threshold[0]} W/m&#178; and ${serverSettings.threshold[1]} W/m&#178;`;
      start = true;
    }
  }
  else {
    passLabel.innerHTML = 'Enter Passcode';
    passLabel.style.color = '#03e9f4';
  }
});

function reconnect() {
  if(start) {
    document.querySelector('#sun-reading').textContent = `Connection Closed`;
    document.querySelector('.sun-light-summary').innerHTML = `<i>Reconnecting...</i>`
  }
  setTimeout(() => {
    socket = new WebSocket(websocketUrl);
  }, 2000)
}

// Connection opened
socket.addEventListener('open', function (event) {
  console.log('WebSocket connection established');
});

// Connection closed
socket.addEventListener('close', () => {
  console.log('WebSocket connection closed');
  reconnect();
});

// Error occurred
socket.addEventListener('error', (e) => {
  const { data } = e;
  reconnect();
});

let passkey = true;

// Receive message from server
socket.addEventListener('message', (e) => {
  const { data } = e;
  const jsonData = JSON.parse(data);

  if (jsonData.hasOwnProperty("voltageInMemory")) {
    const threshold = jsonData.voltageInMemory;
    for (let i = 0; i < threshold.length; i++) {
      console.log(`Voltage in Memory ${threshold[i]}`);
      serverSettings.threshold[i] = threshold[i];
    }
  }

  if (jsonData.hasOwnProperty("passCode")) {
    const passCode = jsonData.passCode;
    console.log(passCode);
    if (passkey) {
      passkey = false;
      pass = passCode;
    }
  }

  if (jsonData.hasOwnProperty("irr")) {
    if (start) {
      const irradiance = jsonData.irr;
      document.querySelector('#sun-reading').innerHTML = `${irradiance} W/m&#178;`;
      document.querySelector('.sun-light-summary').innerHTML = irradiance > 970 ? 'Bright Light' : 'Low light';
    }
  }

  if (jsonData.hasOwnProperty("clients")) {
    msg = jsonData.clients;
    console.log(msg[0]);
    console.log(msg[1]);
    if(start) {
      clientConnected();
    }
  } 
});


// Example usage: sending a message to the server
/* function sendMessage(message) {delay(500);
  if (socket.readyState === WebSocket.OPEN) {
    socket.send(message);
    console.log('Sent message to server:', message);
  }
} */

// Example usage: sending a message to the server
// sendMessage('Hello from the browser!');

/* window.addEventListener('scroll', footerSectionHandler.toggleSectionBg); */

let led = true;

function setLightThreshold(labelTxt, lightInMem, formID) {
  const form = document.createElement('form');
  form.setAttribute('id', formID);
  form.className = 'set-irr-form';
  const label = document.createElement('label');
  label.setAttribute('for', `${formID}-input`);
  label.textContent = labelTxt;
  
  const input = document.createElement('input');
  input.setAttribute('type', 'number');
  input.setAttribute('id', `${formID}-input`); 
  input.setAttribute('name', 'entry');
  input.setAttribute('min', '0');
  input.setAttribute('max', '1000');
  input.setAttribute('required', 'true');
  input.setAttribute('value', lightInMem);
  
  const submitButton = document.createElement('input');
  submitButton.setAttribute('type', 'submit');
  if (formID !== 'max-entry') {
    submitButton.setAttribute('value', 'Next');
    submitButton.setAttribute('id', 'submit');
  }
  else {
    submitButton.setAttribute('value', 'Submit');
    submitButton.setAttribute('id', 'submit-next');
  }
  
  const button = document.createElement('button');
  button.setAttribute('type','button');
  button.setAttribute('id','back');
  button.innerText = 'Back'; 
  
  const backkBtn = document.createElement('button');
  backkBtn.setAttribute('id', 'back-btn');
  backkBtn.innerHTML = icons.enter;
  
  if (formID !== 'max-entry')
    form.append(label, input, submitButton, backkBtn);
  else {
    form.append(label, input, button, submitButton, backkBtn);
  }
  document.querySelector('.item1').appendChild(form);
}

window.addEventListener('submit', (e) => {
  e.preventDefault(); // Prevent form submission
  const { target } = e;
  console.log(target);
  // Check if the target matches a specific selector
  if (target.matches('#min-entry')) {
    const input = target.querySelector('input');
    if (input) {
      // Extract the input value
      clientSettings.threshold[0] = parseInt(input.value, 10);
      document.querySelector('#min-entry').remove();
      setLightThreshold('Set maximum intensity', serverSettings.threshold[1], 'max-entry');
      const form = document.querySelector('#max-entry');
      const firstChild = document.querySelector('#max-entry label')
      const h2 = document.createElement('h2');
      const div = document.createElement('div');
      div.setAttribute('id', 'error-msg');
      div.appendChild(h2);
      h2.innerHTML = `${clientSettings.threshold[0]} W/m&#178;`;
      form.insertBefore(div, firstChild);
    }
  }
  else if (target.matches('#max-entry')) {
    const input = target.querySelector('input');
    if (input) {
      const valueInBase10 = parseInt(input.value, 10);
      if (valueInBase10 > clientSettings.threshold[0]) {
        clientSettings.threshold[1] = valueInBase10;
        socket.send(clientSettings);
        document.querySelector('#max-entry').remove();
        createThresholdBtn();
        sucessSubmit = true;
        // create successful display
      }
      else {
          if(sucessSubmit) {
          const errorMsg = document.createElement('p');
          errorMsg.innerHTML = `Minimum can't be greater than maximum irradiance!`;
          document.querySelector('#error-msg').appendChild(errorMsg);
          sucessSubmit = false;
        }
      }
    }
  }
  console.log(clientSettings);
});

document.querySelector('body').addEventListener('touchstart', (e) => {
  console.log(e.target);
  const { target } = e;
  if (target === document.querySelector('.item1') && led) {
    const message = { action: 'led_on' }
    const jsonString = JSON.stringify(message);
    socket.send(jsonString);
    led = false;
  }
  else if (target === document.querySelector('.item1') && led === false) {
    const message = { action: 'led_off'}
    const jsonString = JSON.stringify(message);
    socket.send(jsonString);
    led = true;
  }
  else if (target.matches('#threshold, #threshold *')) {
    console.log("Im here");
    setLightThreshold('Set minimum intensity', serverSettings.threshold[0], 'min-entry')
    document.querySelector('#threshold').remove();
  }
  else if (target.matches('#back')) {
    document.querySelector('#max-entry').remove();
    setLightThreshold('Set minimum intensity', clientSettings.threshold[0], 'min-entry');
  }

  else if (target.matches('#back-btn, #back-btn *')) {
    const container = document.querySelector('.item1');
    const maxEntry = container.querySelector('#max-entry');
    const minEntry = container.querySelector('#min-entry');
    createThresholdBtn();
    if (maxEntry) 
      maxEntry.remove();
    else if (minEntry) 
      minEntry.remove();
  }
});


