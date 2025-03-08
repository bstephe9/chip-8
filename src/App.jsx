import React, { useEffect, useRef, useState } from "react"
import createModule from "../public/chip8.js"
import "./App.css"

function App() {
  const moduleRef = useRef(null)
  const [roms, setRoms] = useState([]);

  useEffect(() => {
    const initModule = async () => {
      const moduleInstance = await createModule()
      moduleInstance["canvas"] = document.getElementById("canvas")
      moduleRef.current = moduleInstance
      const romList = getRoms(moduleInstance);
      setRoms(romList);
    };

    initModule()
  }, []);

  const handleClick = (event) => {
    moduleRef.current.callMain(["./roms/Airplane.ch8"])
  };

  const reload = (romPath) => {
    moduleRef.current.ccall('reload', null, ['string'], [`./roms/${romPath}`])
  }

  const getRoms = (moduleInstance) => {
    if (!moduleInstance) return [];
    const romDir = moduleInstance.FS.readdir("/roms");
    return romDir.filter((file) => file !== "." && file !== "..");
  };

  return (
    <div className="app">
      <h1>CHIP-8 Emulator</h1>
      <button style={{ display: "block" }} onClick={handleClick}>Click me</button>
      <ul>
        {roms.map((rom, index) => (
          <li key={index} onClick={() => reload(rom)} style={{ cursor: "pointer", color: "blue" }}>
            {rom}
          </li>
        ))}
      </ul>
      <canvas style={{ background: "blue" }} id="canvas" width="800" height="600"></canvas>
    </div>
  )
}

export default App