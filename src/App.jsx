import React, { useEffect, useRef, useState } from "react"
import createModule from "../public/chip8.js"
import "./App.css"
import RomList from "./components/RomList/RomList.jsx"

function App() {
  const moduleRef = useRef(null)
  const [roms, setRoms] = useState([])
  const [programStarted, setProgramStarted] = useState(false)

  useEffect(() => {
    const initModule = async () => {
      const moduleInstance = await createModule()
      moduleInstance["canvas"] = document.getElementById("canvas")
      moduleRef.current = moduleInstance
      const romList = getRoms(moduleInstance)
      setRoms(romList)
    }

    initModule()
  }, [])

  const getRoms = (moduleInstance) => {
    if (!moduleInstance) return []
    const romDir = moduleInstance.FS.readdir("/roms")
    return romDir.filter((file) =>
      file !== "." &&
      file !== ".." &&
      file.endsWith(".ch8")
    )
  }

  const handleClick = (romFile) => {
    !programStarted ? startCHIP8(romFile) : reloadCHIP8(romFile)
  }

  const startCHIP8 = (romFile) => {
    moduleRef.current.callMain([`./roms/${romFile}`])
    setProgramStarted(true)
  }

  const reloadCHIP8 = (romFile) => {
    moduleRef.current.ccall('reload', null, ['string'], [`./roms/${romFile}`])
  }

  return (
    <>
      <h1 style= {{ marginTop: "16px" }}>CHIP-8 Emulator</h1>
      <div className="content">
        <canvas style={{ background: "black" }} id="canvas" width="832"></canvas>
        <RomList itemList={roms} handleClick={handleClick} />
      </div>
    </>
  )
}

export default App