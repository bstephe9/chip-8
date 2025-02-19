console.log("Before");

const roms = FS.readdir("/roms")
  .filter(item => item != "." && item != "..");

console.log(roms);

function addRomLinks() {
  console.log("Adding rom links...");
  for (let i = 0; i < roms.length; i++) {
    let link = document.createElement("a");
    link.text = roms[i];
  }
}

addRomLinks();

// document.getElementById("main").onclick = () => {
    
//   let randomItem = roms[Math.floor(Math.random() * roms.length)];
//   console.log(roms);
//   console.log(randomItem);
//   Module['arguments_'] = [`/roms/${randomItem}`];
//   Module.callMain(Module['arguments_']);
// }