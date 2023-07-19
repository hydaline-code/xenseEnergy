const icons = {
  solarPower: '<svg xmlns="http://www.w3.org/2000/svg" height="48" viewBox="0 -960 960 960" width="48"><path d="m80-80 80-400h640l80 400H80Zm40-740v-60h120v60H120Zm34 680h296v-110H176l-22 110Zm80-460-43-42 85-85 43 42-85 85Zm-46 290h262v-110H210l-22 110Zm292-390q-74 0-125-52.5T300-880h60q3 50 37 85t83 35q49 0 83-35t37-85h60q-4 75-55 127.5T480-700Zm0-180Zm-30 360v-120h60v120h-60Zm60 380h296l-22-110H510v110Zm0-170h262l-22-110H510v110Zm216-291-85-85 42-42 86 84-43 43Zm-6-219v-60h120v60H720Z"/></svg>',
  enter: `<svg width="34" height="27" viewBox="0 0 34 27" fill="none" xmlns="http://www.w3.org/2000/svg">
    <path d="M19.0401 0.986284C19.4794 0.547044 20.0753 0.300293 20.6965 0.300293C21.3178 0.300293 21.9136 0.547044 22.353 0.986284L32.8964 11.5297C33.3356 11.969 33.5824 12.5649 33.5824 13.1862C33.5824 13.8074 33.3356 14.4033 32.8964 14.8426L22.353 25.386C21.9111 25.8128 21.3193 26.049 20.705 26.0436C20.0907 26.0383 19.503 25.7919 19.0686 25.3575C18.6342 24.9231 18.3878 24.3354 18.3824 23.7211C18.3771 23.1068 18.6133 22.515 19.0401 22.0731L25.3825 15.5291H3.12423C2.50283 15.5291 1.90688 15.2823 1.46749 14.8429C1.0281 14.4035 0.78125 13.8076 0.78125 13.1862C0.78125 12.5648 1.0281 11.9688 1.46749 11.5294C1.90688 11.09 2.50283 10.8432 3.12423 10.8432H25.3825L19.0401 4.29925C18.6008 3.85988 18.3541 3.26404 18.3541 2.64277C18.3541 2.02149 18.6008 1.42566 19.0401 0.986284Z" fill="#3AA945"/>
    </svg>`,
  settings: '<svg xmlns="http://www.w3.org/2000/svg" height="48" viewBox="0 -960 960 960" width="48"><path d="m388-80-20-126q-19-7-40-19t-37-25l-118 54-93-164 108-79q-2-9-2.5-20.5T185-480q0-9 .5-20.5T188-521L80-600l93-164 118 54q16-13 37-25t40-18l20-127h184l20 126q19 7 40.5 18.5T669-710l118-54 93 164-108 77q2 10 2.5 21.5t.5 21.5q0 10-.5 21t-2.5 21l108 78-93 164-118-54q-16 13-36.5 25.5T592-206L572-80H388Zm92-270q54 0 92-38t38-92q0-54-38-92t-92-38q-54 0-92 38t-38 92q0 54 38 92t92 38Zm0-60q-29 0-49.5-20.5T410-480q0-29 20.5-49.5T480-550q29 0 49.5 20.5T550-480q0 29-20.5 49.5T480-410Zm0-70Zm-44 340h88l14-112q33-8 62.5-25t53.5-41l106 46 40-72-94-69q4-17 6.5-33.5T715-480q0-17-2-33.5t-7-33.5l94-69-40-72-106 46q-23-26-52-43.5T538-708l-14-112h-88l-14 112q-34 7-63.5 24T306-642l-106-46-40 72 94 69q-4 17-6.5 33.5T245-480q0 17 2.5 33.5T254-413l-94 69 40 72 106-46q24 24 53.5 41t62.5 25l14 112Z"/></svg>',
};

const websocketUrl = 'ws://192.168.1.1/ws';
let socket = new WebSocket(websocketUrl);

const serverSettings = { threshold: [] };
const clientSettings = { threshold: [] };
const main = document.querySelector('main');
const irrUnit = '<span class = \'small-txt\'>W/m&#178;</span>';
let led = true;
let pass = '1234';
let start = false;
let msg = null;
let sucessSubmit = true;
let passkey = true;
const green = '#3aa945';

function clientConnected() {
  let content;
  if (msg[0] === 2) { content = 'Only you and a user are connected to this Power Manager'; } else if
  (msg[0] > 2) { content = `You and ${msg[0] - 1} are connected are connected to Power Manager`; } else { content = 'You are connected to your Energy Manager'; }
  if (main.querySelector('#no-clients')) {
    main.querySelector('#no-clients').textContent = content;
  } else {
    const container = document.createElement('p');
    container.id = 'no-clients';
    container.textContent = content;
    main.appendChild(container);
  }
}

function createThresholdBtn() {
  const article2 = document.createElement('article');
  article2.id = 'threshold';
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
  h2.innerHTML = 'Sunlight intensity threshold for power activation';
  item1.append(article);
}

function createHomePage() {
  const div = document.createElement('div');
  div.className = 'sun-reading-container';
  const h2 = Object.assign(document.createElement('h1'), { id: 'sun-reading' });
  h2.textContent = '0';
  const p = document.createElement('p');
  p.className = 'sun-light-summary';
  div.append(h2, p);
  const section = document.createElement('section');
  section.id = 'home';
  const article = document.createElement('article');
  article.className = 'item';
  article.id = 'item';
  const article1 = document.createElement('article');
  article1.className = 'item1 item';
  const article2 = document.createElement('article');
  article2.className = 'item2 item';
  section.append(article2, article1, article);
  main.append(div, section);
  main.style.background = 'linear-gradient(to bottom right, #ffffff, #f2f2f2)';
  document.querySelector('.item2').innerHTML = 'Maximum normal surface irradiance is approximately 1000 W/m&#178; at sea level on a clear day';

  item1Content();
  createThresholdBtn();
  window.scrollTo(0, 0);
}

// Loging Page
setTimeout(() => {
  document.querySelector('#logo-anim').style.left = '28px';
}, 1000);

document.querySelector('#passcode').addEventListener('keyup', (e) => {
  const { target } = e;
  const passLabel = document.querySelector('#pass-lebel');
  if (target.value.length >= 4) {
    if (pass < 1) {
      passLabel.innerHTML = 'Your device is not connected to your Energy Manager. Check your WiFi and refresh page';
      passLabel.style.color = '#f28500';
    } else if (target.value !== pass) {
      passLabel.innerHTML = 'Passcode Invalid';
      passLabel.style.color = '#f28500';
    } else {
      document.querySelector('body').classList.remove('body');
      document.querySelector('.login-box').remove();
      createHomePage();
      clientConnected();
      document.querySelector('.nrTxt').innerHTML = `Power activation for the connected Energy manager is between: <br> 
        ${serverSettings.threshold[0]} ${irrUnit} and ${serverSettings.threshold[1]} ${irrUnit}`;
      start = true;
    }
  } else {
    passLabel.innerHTML = 'Enter Passcode';
    passLabel.style.color = '#03e9f4';
  }
});

function reconnect() {
  if (start) {
    document.querySelector('#sun-reading').textContent = 'Connection lost';
    document.querySelector('.sun-light-summary').innerHTML = '<i>Reconnecting...</i>';
    document.querySelector('footer span').innerHTML = '';
  }
  setTimeout(() => {
    socket = new WebSocket(websocketUrl);
  }, 1000);
}

// Connection opened
socket.addEventListener('open', () => {
});

// Connection closed
socket.addEventListener('close', () => {
  reconnect();
});

socket.addEventListener('error', () => {
  reconnect();
});

socket.addEventListener('message', (e) => {
  const { data } = e;
  const jsonData = JSON.parse(data);

  if (Object.prototype.hasOwnProperty.call(jsonData, 'voltageInMemory')) {
    const threshold = jsonData.voltageInMemory;
    for (let i = 0; i < threshold.length; i += 1) {
      serverSettings.threshold[i] = threshold[i];
    }
    if (start) {
      document.querySelector('.nrTxt').innerHTML = `Power activation for the connected Energy manager is between: <br> 
        ${serverSettings.threshold[0]} ${irrUnit} and ${serverSettings.threshold[1]} ${irrUnit}`;
    }
  }

  if (Object.prototype.hasOwnProperty.call(jsonData, 'passCode')) {
    const { passCode } = jsonData;
    if (passkey) {
      passkey = false;
      pass = passCode;
    }
  }

  if (Object.prototype.hasOwnProperty.call(jsonData, 'irr')) {
    if (start) {
      const irradiance = jsonData.irr;
      document.querySelector('#sun-reading').innerHTML = `${irradiance} ${irrUnit}`;
      document.querySelector('.sun-light-summary').innerHTML = irradiance > 970 ? 'Bright Light' : 'Low light';
    }
  }

  if (Object.prototype.hasOwnProperty.call(jsonData, 'clients')) {
    msg = jsonData.clients;
    if (start) {
      clientConnected();
    }
  }

  if (Object.prototype.hasOwnProperty.call(jsonData, 'availableWifi')) {
    console.log(jsonData); //-------------------------------------------------------
  }
 });

function setLightThreshold(labelTxt, lightInMem, formID) {
  const form = document.createElement('form');
  form.id = formID;
  form.className = 'set-irr-form';
  const fieldset1 = document.createElement('fieldset');
  fieldset1.className = 'fs1';
  const label = document.createElement('label');
  label.for = `${formID}-input`;
  label.textContent = labelTxt;
  fieldset1.append(label);

  const input = document.createElement('input');
  input.type = 'number';
  input.id = `${formID}-input`;
  input.name = 'entry';
  input.min = '0';
  input.max = '1000';
  input.required = true;
  input.value = lightInMem;

  const fieldset = document.createElement('fieldset');
  fieldset.className = 'fs2';

  const submitButton = document.createElement('button');
  submitButton.type = 'submit';
  if (formID !== 'max-entry') {
    submitButton.value = 'Next';
    submitButton.innerText = 'Next';
    submitButton.id = 'submit';
  } else {
    submitButton.value = 'Submit';
    submitButton.innerText = 'Submit';
    submitButton.id = 'submit-next';
  }

  const button = document.createElement('button');
  button.type = 'button';
  button.id = 'back';
  button.innerText = 'Back';

  const backBtn = document.createElement('button');
  backBtn.id = 'back-btn';
  backBtn.innerHTML = icons.enter;

  if (formID !== 'max-entry') { 
    fieldset.append(submitButton)
    form.append(fieldset1, input, fieldset, backBtn); } else {
      fieldset.append(button, submitButton)
      form.append(fieldset1, input, fieldset, backBtn);
  }
  document.querySelector('#item').appendChild(form);
}

async function checkSettingsMatch(serverSettings, clientSettings) {
  return new Promise((resolve) => {
    setTimeout(() => {
      resolve(
        serverSettings.threshold[0] === clientSettings.threshold[0]
        && serverSettings.threshold[1] === clientSettings.threshold[1],
      );
    }, 2000);
  });
}

function formDeliveredMessage() {
  const button = document.createElement('button');
  button.innerHTML = icons.enter;
  button.id = 'back-btn';
  const article = document.createElement('article');
  article.className = 'form-submit';
  const msg = document.createElement('div');
  msg.innerHTML = '<div class="lds-ring"><div></div><div></div><div></div><div></div></div>';
  article.append(msg);
  document.querySelector('#item').append(article);
  (async () => {
    const settingsMatch = await checkSettingsMatch(serverSettings, clientSettings);
    console.log(settingsMatch);
    if (clientSettings && serverSettings) {
      msg.innerHTML = 'Update was successful';
      msg.style.color = green;
      const p = document.createElement('p');
      p.innerHTML = `Sunlight threshold is set to`;
      const h2 = document.createElement('h2');
      h2.innerHTML = `${serverSettings.threshold[0]}/${serverSettings.threshold[1]}`;
      article.append(p, h2, button);
    } else {
      msg.innerHTML = 'Error';
      msg.style.color = 'red';
    }
  })();
}

window.addEventListener('submit', (e) => {
  e.preventDefault();
  const { target } = e;
  if (target.matches('#min-entry')) {
    const input = target.querySelector('input');
    if (input) {
      clientSettings.threshold[0] = parseInt(input.value, 10);
      document.querySelector('#min-entry').remove();
      setLightThreshold('Set max. sunlight threshold', serverSettings.threshold[1], 'max-entry');
      const form = document.querySelector('#max-entry');
      const firstChild = document.querySelector('#max-entry label');
      const h2 = document.createElement('h2');
      const fs1 = document.querySelector('.fs1');
      fs1.insertBefore(h2, firstChild);
      h2.innerHTML = `${clientSettings.threshold[0]} ${irrUnit}`;
      form.insertBefore(fs1, document.querySelector('.fs1'));
    }
  } else if (target.matches('#max-entry')) {
    const input = target.querySelector('input');
    if (input) {
      const valueInBase10 = parseInt(input.value, 10);
      if (valueInBase10 > clientSettings.threshold[0]) {
        clientSettings.threshold[1] = valueInBase10;
        const clientSettingsJSON = JSON.stringify(clientSettings);
        socket.send(clientSettingsJSON);
        document.querySelector('#max-entry').remove();
        sucessSubmit = true;

        // create successful display ///////////////////////////////
        formDeliveredMessage();
      } else if (sucessSubmit) {
        const errorMsg = document.createElement('p');
        errorMsg.innerHTML = 'Min. threshold can\'t exceed max. threshold';
        const lastElement = document.querySelector('#max-entry').lastElementChild;
        const parent = document.querySelector('#max-entry');
        parent.insertBefore(errorMsg, lastElement.previousSibling);
      }
    }
  }
});

document.querySelector('body').addEventListener('touchstart', (e) => {
  const { target } = e;
  if (target === document.querySelector('.item1') && led) {
    const message = { action: 'led_on' };
    const jsonString = JSON.stringify(message);
    socket.send(jsonString);
    led = false;
  } else if (target === document.querySelector('.item1') && led === false) {
    const message = { action: 'led_off' };
    const jsonString = JSON.stringify(message);
    socket.send(jsonString);
    led = true;
  } else if (target.matches('#threshold, #threshold *')) {
    setLightThreshold('Set min. sunlight threshold', serverSettings.threshold[0], 'min-entry');
    document.querySelector('#threshold').remove();
  } else if (target.matches('#back')) {
    document.querySelector('#max-entry').remove();
    setLightThreshold('Set min. sunlight threshold', clientSettings.threshold[0], 'min-entry');
  } else if (target.matches('#back-btn, #back-btn *')) {
    const item = document.querySelector('#item');
    while (item.firstChild) { item.removeChild(item.firstChild); }

    createThresholdBtn();
  } else if (target.matches('footer, footer *')) {
    const msg = 'getWifiScan';
    socket.send(msg);
  }
});