import * as React from 'react'
import Box from '@mui/material/Box'
import List from '@mui/material/List'
import ListItem from '@mui/material/ListItem'
import ListItemAvatar from '@mui/material/ListItemAvatar'
import ListItemText from '@mui/material/ListItemText'
import Avatar from '@mui/material/Avatar'
import Typography from '@mui/material/Typography'
import FolderIcon from '@mui/icons-material/Folder'

export default function RomList({ itemList, handleClick }) {
  return (
    <Box sx={{ paddingLeft: 2, flexGrow: 1, border: 2, }}>
      <Typography sx={{ mt: 1.5, mb: 2 }} variant="h6" component="div">
        <strong>ROMS</strong>
      </Typography>
      <List
        sx={{
          '& .MuiListItem-root:hover': {
            bgcolor: 'gray',
            '&, & .MuiListItem-root': {
              color: 'yellow',
            },
          },
          overflowY: "scroll",
          maxHeight: "70vh",
        }}>
        {itemList.map((item) => (
          <ListItem
            key={item}
            onClick={() => handleClick(item)}
            style={{ cursor: "pointer" }}>
            <ListItemAvatar>
              <Avatar>
                <FolderIcon />
              </Avatar>
            </ListItemAvatar>
            <ListItemText primary={item} />
          </ListItem>
        ))}
      </List>
    </Box>
  )
}
