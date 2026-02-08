# global_copy_source.py
# This script should be placed in ~/.platformio/scripts/

import os
import shutil
Import("env")

def post_upload_action(source, target, env):
    """
    This function is called after a successful upload.
    It copies the project's main.cpp to a folder
    within the same project directory.
    """
    # Get the project directory from the environment variables
    project_dir = env.get("PROJECT_DIR")
    if not project_dir:
        print("Error: Could not determine the project directory.")
        return

    print("Copying main.cpp to the project's folder...")

    # Define source and destination paths relative to the project
    source_file = os.path.join(project_dir, "src", "main.cpp")
    destination_folder = project_dir
    destination_file = os.path.join(destination_folder, "Last Uploaded Code.cpp")

    # Check if the source file exists
    if not os.path.isfile(source_file):
        print(f"Error: Source file not found at {source_file}")
        return

    # Ensure the destination folder exists, create it if it doesn't
    if not os.path.isdir(destination_folder):
        try:
            os.makedirs(destination_folder)
            print(f"Created directory: {destination_folder}")
        except Exception as e:
            print(f"Error creating directory {destination_folder}: {e}")
            return

    # Copy and rename the file
    try:
        shutil.copyfile(source_file, destination_file)
        print(f"Successfully copied source to {destination_file}")
    except Exception as e:
        print(f"Error copying file: {e}")

# Register the function to be called after the 'upload' command
env.AddPostAction("upload", post_upload_action)