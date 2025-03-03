import React, { useEffect, useRef } from "react"
import createModule from "../public/chip8.js"
import "./App.css"

function App() {
  const moduleRef = useRef(null);

  useEffect(() => {
    const initModule = async () => {
      console.log("UseEffect called...");
      const moduleInstance = await createModule();
      moduleInstance["canvas"] = document.getElementById("canvas");
      moduleRef.current = moduleInstance;
      console.log("Module created...");
    };

    initModule();
  }, []);

  const handleClick = (event) => {
    console.log('Button clicked!', event);
    moduleRef.current.callMain(["./roms/Airplane.ch8"])
  };

  return (
    <div className="app">
      <h1>CHIP-8 Emulator</h1>
      <button style={{ display: "block" }} onClick={handleClick}>Click me</button>
      <canvas style={{ background: "blue" }} id="canvas" width="800" height="600"></canvas>
    </div>
  )
}

export default App