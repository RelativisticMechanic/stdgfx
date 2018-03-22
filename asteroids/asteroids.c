#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdgfx.h>

#define pScale 1
#define pA 90.0f
#define pVel_max 300.0f
#define pVel_theta 3.0f

#define stMax 1024
// Asteroid properties
#define aVerts 20
#define aRadius 80.0f
#define aVel_min 20.0f
#define aVel_max 60.0f

#define bSize 2
#define bVel pVel_max * 2
// 200 millisecond difference between shots
#define shot_interval 200.0f

bool game_started = false;

double pX = 0;
double pY = 0;
// Velocity
double pVel_x = 0.01f;
double pVel_y = 0.0f;

int score = 0;

// Theme id
int theme_id = 0;

// Angle made by the player with the horizontal axis
double pTheta = 0.1f;

// Player ship model
double pV_x[3] = { pScale*0.0f, pScale*(-5.0f), (pScale)*5.0f };
double pV_y[3] = { pScale*(-10.0f), pScale*(5.0f), pScale*5.0f };

typedef struct {
    double x;
    double y;
    double vx;
    double vy;
    int active;
} bullet;

bullet* bullets = NULL;
int nBullets = 0;

typedef struct {
    double x;
    double y;
    double radius;
    double vx;
    double vy;
    double vertx[aVerts];
    double verty[aVerts];
    int active;
} asteroid;

asteroid* asteroids = NULL;
int nAsteroids = 0;

double signum(double x)
{
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int random_range(int M, int N)
{
    return (M + rand() / (RAND_MAX / (N - M + 1) + 1));
}

int random_sign(void)
{
    int r = rand();
    if(r % 2)
        return -1;
    else
        return 1;
}

void add_bullet(double x, double y, double vx, double vy) {
    if(!bullets)
    {
        bullets = calloc(20, sizeof(bullet)*20);
        nBullets = 20;
    }

    for(int i = 0; i < nBullets; i++)
    {
        if(bullets[i].active == 0)
        {
            bullets[i].active = 1;
            bullets[i].x = x; bullets[i].y = y; 
            bullets[i].vx = vx; bullets[i].vy = vy;
            return;
        }
    }

    // Not found, reallocate
    bullet* new_bullets = calloc(nBullets + 20, sizeof(bullet)*(nBullets + 20));
    gfx_ok(!new_bullets, "out of memory :D\n");
    for(int i = 0; i < nBullets; i++)
    {
        new_bullets[i].active = bullets[i].active;
        new_bullets[i].vx = bullets[i].vx;
        new_bullets[i].vy = bullets[i].vy;
        new_bullets[i].x = bullets[i].x;
        new_bullets[i].y = bullets[i].y;
    }
    free(bullets);
    for(int i = nBullets; i < nBullets + 20; i++)
    {
        if(bullets[i].active == 0)
        {
            bullets[i].active = 1;
            bullets[i].x = x; bullets[i].y = y; 
            bullets[i].vx = vx; bullets[i].vy = vy;
            break;
        }
    }
    bullets = new_bullets;
    nBullets += 20;

}

void remove_bullet(int idx)
{
    if(idx >= nBullets)
        return;
    else
        bullets[idx].active = 0;
}

void generate_vertices(int i, double radius)
{
    for(int j = 0; j < aVerts; j++)
    {
        double angle = ((double)j/(double)aVerts) * M_PI * 2;
        asteroids[i].vertx[j] = radius * cos(angle) + random_sign() * random_range(0, 4);
        asteroids[i].verty[j] = radius * sin(angle) + random_sign() * random_range(0, 4);
    }
}
void add_asteroid(double x, double y, double radius, double vx, double vy) {
    if(!asteroids)
    {
        asteroids = calloc(20, sizeof(asteroid)*20);
        nAsteroids = 20;
    }

    for(int i = 0; i < nAsteroids; i++)
    {
        if(asteroids[i].active == 0)
        {
            asteroids[i].active = 1;
            asteroids[i].x = x; asteroids[i].y = y; 
            asteroids[i].vx = vx; asteroids[i].vy = vy;
            asteroids[i].radius = radius;
            generate_vertices(i, radius);
            return;
        }
    }

    // Not found, reallocate
    asteroid* new_asteroids = calloc(nAsteroids + 20, sizeof(asteroid)*(nAsteroids + 20));
    gfx_ok(!new_asteroids, "out of memory :D\n");
    for(int i = 0; i < nAsteroids; i++)
    {
        new_asteroids[i].active = new_asteroids[i].active;
        new_asteroids[i].vx = new_asteroids[i].vx;
        new_asteroids[i].vy = new_asteroids[i].vy;
        new_asteroids[i].x = new_asteroids[i].x;
        new_asteroids[i].y = new_asteroids[i].y;
        new_asteroids[i].radius = new_asteroids[i].radius;
    }
    free(asteroids);
    for(int i = nAsteroids; i < nAsteroids + 20; i++)
    {
        if(asteroids[i].active == 0)
        {
            asteroids[i].active = 1;
            asteroids[i].x = x; asteroids[i].y = y; 
            asteroids[i].vx = vx; asteroids[i].vy = vy;
            asteroids[i].radius = radius;
            generate_vertices(i, radius);
            break;
        }
    }
    asteroids = new_asteroids;
    nAsteroids += 20;

}

void remove_asteroid(int idx)
{
    if(idx >= nAsteroids)
        return;
    else
        asteroids[idx].active = 0;
}

int count_asteroids(void)
{
    if(asteroids == NULL) return 0;

    int count = 0;
    for(int i = 0; i < nAsteroids; i++)
    {
        if(asteroids[i].active == 1)
            count++;
    }
    return count;
}

void reset_game(void)
{
    free(asteroids);
    free(bullets);
    gfx_stop_sound(theme_id);
    asteroids = NULL;
    bullets = NULL;
    nAsteroids = 0;
    nBullets = 0;
    pX = gfx_width / 2;
    pY = gfx_width / 2;
    pVel_x = 0.01f;
    pVel_y = 0.0f;
    pTheta = 0.01f;
    int old_score = score;
    score = 0;
    // Game over man
    while(true)
    {
        gfx_clear(0,0,0,0);
        gfx_text_cursor(gfx_terminal_max_x / 2 - 25, gfx_terminal_max_y / 2);
        gfx_printf("Game over, your score: %d, press CTRL to continue", old_score);
        if(gfx_iskey(GFX_CTRL))
        {
            gfx_clear(0,0,0,0);
            gfx_update(); // Refresh input buffer
            return;
        }
        gfx_update();
    }
}
int gfx_main(int argc, char** argv)
{
    gfx_resize(800, 600, 32);
    gfx_pixel_wrap(1);
    
    SOUND* theme = gfx_load_sound("./quake2.ogg");
    IMAGE* logo = gfx_load_image("./asteroids.png");

    pX = gfx_width / 2;
    pY = gfx_height / 2;
    // Make stars
    double starX[stMax];
    double starY[stMax];
    
    // Our stars
    for(int i = 0; i < stMax; i++)
    {
        starX[i] = random_range(0, gfx_width);
        starY[i] = random_range(0, gfx_height);
    }
    uint32_t shoot_timer = gfx_ticks();
    double game_timer = (double)gfx_ticks() / 1000.0f;
    double time_elapsed = 0;
    add_asteroid(20, 25, 80, random_sign()*random_range(aVel_min, aVel_max), random_sign()*random_range(aVel_min, aVel_max));

    while(true)
    {
        time_elapsed = (gfx_ticks() / 1000.0f) - game_timer;
        game_timer = gfx_ticks() / 1000.0f;

        gfx_clear(0, 0, 0, 0);

        if(!game_started)
        {
            gfx_draw_image(logo, gfx_width/2 - 128, gfx_height/2 - 128 , 256, 256);
            gfx_text_cursor(gfx_terminal_max_x / 2 - 8, gfx_terminal_max_y / 2 - 20);
            gfx_printf("GFXDROIDS 0.1\n");
            gfx_text_cursor(gfx_terminal_max_x / 2 - 20, gfx_terminal_max_y / 2);
            gfx_printf("Press CTRL to begin the space carnage!\n");

            if(gfx_iskey(GFX_CTRL))
            {
                // PLAY IT
                theme_id = gfx_play_sound(theme);
                game_started = true;
            }
            gfx_update();
            continue;
        }
        // Calculate the vertices of the triangle
        if(gfx_iskey(GFX_D))
        {
            pTheta += pVel_theta * time_elapsed;
        }
        if(gfx_iskey(GFX_A))
        {
            pTheta -= pVel_theta * time_elapsed;
        }

        if(gfx_iskey(GFX_W))
        {
            pVel_x += pA*sin(pTheta)*time_elapsed;
            pVel_y -= pA*cos(pTheta)*time_elapsed;
        }

        if(gfx_iskey(GFX_S))
        {
            pVel_x -= pA*sin(pTheta)*time_elapsed;
            pVel_y += pA*cos(pTheta)*time_elapsed;
        }

        if(gfx_iskey(GFX_SPACE))
        {
            if(gfx_elapsed(shoot_timer) > shot_interval)
            {
                shoot_timer = gfx_ticks();
                add_bullet(pX, pY, bVel * sin(pTheta), -bVel * cos(pTheta));
            }
        }

        if(fabs(pVel_x) >= pVel_max)
            pVel_x = signum(pVel_x)*pVel_max;

        if(fabs(pVel_y) >= pVel_max)
            pVel_y = signum(pVel_y)*pVel_max;
        
        pX += pVel_x * time_elapsed;
        pY += pVel_y * time_elapsed;

        if(pX >= gfx_width) 
            pX -= gfx_width;
        
        if(pX < 0)
            pX += gfx_width;
            
        if(pY >= gfx_height)
            pY = pY - gfx_height;
        
        if(pY < 0)
            pY = pY + gfx_height;
        
        // Draw stars
        for(int i = 0; i < stMax; i++)
        {
            if(starX[i] >= gfx_width) starX[i] -= gfx_width;
            if(starY[i] >= gfx_height) starY[i] -= gfx_height;
            if(starX[i] < 0) starX[i] += gfx_width;
            if(starY[i] < 0) starY[i] += gfx_height;
            
            int star_color = 0xFF - random_range(100, 150);
            gfx_pixel(starX[i], starY[i], star_color, star_color, star_color, 0xFF);
            // Make the star move with the player
            starX[i] += -pVel_x * time_elapsed;
            starY[i] += -pVel_y * time_elapsed;
        }

        // Draw asteroids and check for collision
        for(int i = 0; i < nAsteroids; i++)
        {
            if(asteroids[i].active == 1)
            {
                if(asteroids[i].radius <= aRadius/4)
                {
                    remove_asteroid(i);
                    continue;
                }
                for(int j = 0; j < aVerts; j++)
                {
                    double X = asteroids[i].x;
                    double Y = asteroids[i].y;
                    if(j == aVerts - 1)
                    {
                        gfx_line(
                        X + asteroids[i].vertx[j], 
                        Y + asteroids[i].verty[j],
                        X + asteroids[i].vertx[0],
                        Y + asteroids[i].verty[0],
                        0xFF, 0xFF, 0xFF, 0xFF);
                    }
                    else
                    {
                        gfx_line(
                        X + asteroids[i].vertx[j], 
                        Y + asteroids[i].verty[j],
                        X + asteroids[i].vertx[j+1],
                        Y + asteroids[i].verty[j+1],
                        0xFF, 0xFF, 0xFF, 0xFF);
                    }
                }
                
                for(int k = 0; k < nBullets; k++)
                {
                    if(bullets[k].active == 0) continue;
                    // Distance from the center
                    double center_distance = sqrtf(
                        (bullets[k].x - asteroids[i].x)*(bullets[k].x - asteroids[i].x) + 
                        (bullets[k].y - asteroids[i].y)*(bullets[k].y - asteroids[i].y));
                    
                    if(center_distance <= asteroids[i].radius)
                    {
                        double x = asteroids[i].x;
                        double y = asteroids[i].y;
                        double vx = asteroids[i].vx;
                        double vy = asteroids[i].vy;
                        double radius = asteroids[i].radius / 2;
                        add_asteroid(x, y, radius, vx, vy);
                        add_asteroid(x - radius, y - radius, radius, -vx, -vy);
                        remove_bullet(k);
                        // HIT!
                        score += 15;
                        remove_asteroid(i);
                    }
                }

                // Collision check the player as well
                double player_distance = sqrtf((pX - asteroids[i].x)*(pX - asteroids[i].x)
                + (pY - asteroids[i].y)*(pY - asteroids[i].y));

                if(player_distance <= asteroids[i].radius)
                {
                    game_started = false; // Game over.
                }

                if(asteroids[i].active == 0) continue;

                asteroids[i].x += asteroids[i].vx * time_elapsed;
                asteroids[i].y += asteroids[i].vy * time_elapsed;

                if(asteroids[i].x >= gfx_width)
                    asteroids[i].x -= gfx_width;

                if(asteroids[i].y >= gfx_height)
                    asteroids[i].y -= gfx_height;

                if(asteroids[i].x < 0)
                    asteroids[i].x += gfx_width;
                    
                if(asteroids[i].y < 0)
                    asteroids[i].y += gfx_height;
            }
        }

        // Check if player was hit
        if(game_started == false)
        {
            reset_game();
            continue;
        }
        // Draw bullets
        for(int i = 0; i < nBullets; i++)
        {
            if(bullets[i].active == 1)
            {
                gfx_circle(bullets[i].x, bullets[i].y, bSize, 0xFF, 0xFF, 0x00, 0xFF);
                bullets[i].x += bullets[i].vx * time_elapsed;
                bullets[i].y += bullets[i].vy * time_elapsed;
                
                if(bullets[i].x >= gfx_width || bullets[i].y >= gfx_height || bullets[i].x < 0 || bullets[i].y < 0)
                    remove_bullet(i);
            }
        }
        // Respawn asteroids
        if(count_asteroids() == 0)
        {
            int to_add = random_range(1, 5);
            for(int i = 0; i < to_add; i++)
            {
                add_asteroid(abs(gfx_width - random_range(0, 20)), abs(gfx_height - random_range(0, 20)), aRadius + random_sign()*random_range(0, 4), random_sign()*random_range(aVel_min, aVel_max), random_sign()*random_range(aVel_min, aVel_max)); 
            }
        }
        // Draw the player finally
        // Compute the vertices of the triangle
        double sV_x[3], sV_y[3];
        for(int i = 0; i < 3; i++)
        {
            sV_x[i] = (pX + pV_x[i]*cos(pTheta) - pV_y[i]*sin(pTheta));
            sV_y[i] = (pY + pV_x[i]*sin(pTheta) + pV_y[i]*cos(pTheta)); 
        }

        gfx_triangle(sV_x[0], sV_y[0], sV_x[1], sV_y[1], sV_x[2], sV_y[2], 0x00, 0xFF, 0x00, 0x00);
        gfx_text_cursor(0,0);
        gfx_printf("Asteriods using libgfx, version 0.1\n");
        gfx_printf("Score: %d\n", score);
        gfx_update();
    }
    return 0;
}
