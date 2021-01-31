# Setup

In order to properly set up the database for use, you must run the setup.sql file to load in the data and create the necessary tables. But before you do this you must download the 'suge.csv' and 'caff.csv' files from the project page and put them in a file where you can find them. After you do this, make sure to edit the setup.sql file to change the location where 'LOAD DATA LOCAL INFILE' command is to the location you placed those files in. After you do this the setup.sql file is all setup and ready to go. So you can hop in the sql terminal and create your database. Call it whatever you want, but remember the name and remember the credentials for the user you are creating it with. After that enter into the database by calling 'USE "database_name"' and then type the command 'source "path/to/setup.sql". That will load all the data in for you and setup tables. The LAST thing you need to do before running the server is to go into the settings.py file and change this part to your own data!

# Databases Authentication
`
DATABASES = {
   'default': {
        'ENGINE': 'django.db.backends.mysql',
        'NAME': 'caffeine_data', # YOUR database name
        'USER': 'tommy', # YOUR login info
        'PASSWORD': 'TommyPass2121!', # YOUR login password
        'HOST': 'localhost',
        'PORT': '80',
    }
}
`

# Running
After that, you can find the manage.py file, and run  the following commands
'python3 manage.py makemigrations'
'python3 manage.py migrate'
'python3 manage.py runserver', and navigate to 'localhost:8000/project' to see our work!