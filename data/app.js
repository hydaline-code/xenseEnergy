
const icons = {
  solarPower: `<svg xmlns="http://www.w3.org/2000/svg" height="48" viewBox="0 -960 960 960" width="48"><path d="m80-80 80-400h640l80 400H80Zm40-740v-60h120v60H120Zm34 680h296v-110H176l-22 110Zm80-460-43-42 85-85 43 42-85 85Zm-46 290h262v-110H210l-22 110Zm292-390q-74 0-125-52.5T300-880h60q3 50 37 85t83 35q49 0 83-35t37-85h60q-4 75-55 127.5T480-700Zm0-180Zm-30 360v-120h60v120h-60Zm60 380h296l-22-110H510v110Zm0-170h262l-22-110H510v110Zm216-291-85-85 42-42 86 84-43 43Zm-6-219v-60h120v60H720Z"/></svg>`,
};


const main = document.querySelector('main');
const header = document.querySelector('header');
const footer = document.querySelector('footer');
const html = document.querySelector('html');

let previousPage = 0;
const savedNav = [];
const pass = '1234';
let start = false;

function createHomePage() {
  const div = document.createElement('div');
  div.className = 'sun-reading-container'
/*  */
  const h2 = Object.assign(document.createElement('h1'), { id: 'sun-reading' });
  h2.textContent ='0';
  const p = document.createElement('p');
  p.innerText = 'Brigth Light';
  div.append(h2, p);
  const section = document.createElement('section');
  section.setAttribute('id', 'home');
  const classNames = ['item2', 'item1'];
  for (let i = 0; i < classNames.length; i++) {
    const article = document.createElement('article');
    article.setAttribute('class', classNames[i], 'item');
    article.classList.add('item');
    section.append(article);
  }
  main.append(div, section);  
}

//Loging Page

setTimeout(() => {
  document.querySelector('#logo-anim').style.left = '28px' 
}, 1000);

document.querySelector('#passcode').addEventListener('keyup', (e) => {
  const { target } = e;
  const passLebel = document.querySelector('#pass-lebel');
  if (target.value.length >= 4) {
    if (target.value !== pass) {
      passLebel.innerHTML = 'Passcode Invalid';
      passLebel.style.color = '#f28500';
    }
    else {
      document.querySelector('body').classList.remove('body');
      document.querySelector('.login-box').remove();
      html.classList.add('html');
      createHomePage();
      start = true
    }
  }
  else {
    passLebel.innerHTML = 'Enter Passcode';
    passLebel.style.color = '#03e9f4';
  }
});

setInterval(() => {
  if(start) {
    const sunReading = document.querySelector('#sun-reading');
    sunReading.innerHTML = `${Math.floor(Math.random() * 11) + 10} W/m`;
}
}, 1000)

//End


/* window.addEventListener('scroll', footerSectionHandler.toggleSectionBg); */

window.addEventListener('touchstart', (e) => {
  console.log(e.target);
  const { target } = e;
});

